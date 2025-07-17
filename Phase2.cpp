#include <iostream>
#include <list>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//My Header Files
#include "HeaderFiles/tiny_obj_loader.h"
#include "HeaderFiles/Model3D.h"
#include "HeaderFiles/Camera.h"
#include "HeaderFiles/VAO.h"
#include "Physics/Vector.h"
#include "Physics/Particle.h"
#include "Physics/Object.h"
#include "Physics/ObjectWorld.h"
#include "Physics/ForceGenerator.h"
#include "Physics/ForceRegistry.h"
#include "Physics/GravityForceGenerator.h"
#include "Physics/DragForceGenerator.h"
#include "Physics/ParticleContact.h"
#include "Physics/ContactResolver.h"
#include "Physics/Phase2_Cable/Cable.h"
#include "Physics/Phase2_Cable/CradleParticleContact.h""

//Springs
#include "Physics/Spring/AnchoredSpring.h"
#include "Physics/Spring/ParticleSpring.h"

using namespace std;
using namespace glm;


// +------------------------+ TIME LIBRARIES +------------------------+
#include<chrono>
using namespace std::chrono_literals;
// We will call this our time between "frames"
constexpr::std::chrono::nanoseconds timestep(16ms);


// +------------------------+ DEVELOPER STUFFS +------------------------+
bool isPaused = false;

// +------------------------+ USER INPUTS +------------------------+
Object* spheres[5];
float initialForce = -70.0f;
float particle_radius = 40.0f;
float particle_gap = 85.0f;
float cableLength = 300.0f;

//printf("Initial Force : ");
//scanf("%f",  &initialForce);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) spheres[0]->setObjVel(initialForce, 0.0f, 0.0f);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800,800, "Nows your chance to be a [[Big Shot]] ! ! !", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();


    // +------------------------ GET USER INPUT ------------------------+
    glfwSetKeyCallback(window, key_callback);

    // skibidii toillet
    printf("Initial Force : ");
    cin >> initialForce;
    printf("Particle Radius : ");
    cin >> particle_radius;
    printf("Particle Gap : ");
    cin >> particle_gap;
    printf("Cable Length : ");
    cin >> cableLength;

    // +------------------------ DECLARE SHADERS ------------------------+
    Shader shader("Shaders/solidColorShader.vert", "Shaders/solidColorShader.frag");

    // +------------------------ DECLARE VAOs ------------------------+
    //Smart pointer that is shared and not copied when passed into the constructor of a class.
    //When out of scope, it will delete itself
    auto sphereVAO = make_shared<VAO>("3D/sphere.obj");

    // +------------------------ DECLARE CAMERA ------------------------+
    float windowWidth = 800.0f;
    float windowHeight = 800.0f;
    float fov = 800.f;
    Camera generalCamera(windowWidth, windowHeight, fov);

    // +------------------------ PHASE 2 ------------------------+
    // +------------------------ DECLARE OBJECTS ------------------------+
    // Spheres
    /*Object* spheres[5];*/
    for (int sphere = 0; sphere < 5; sphere++) spheres[sphere] = new Object(sphereVAO);
    
    // Anchor
    Object* anchors[5];
    for (int a = 0; a < 5; a++) anchors[a] = new Object(sphereVAO);

    //Cables
    physics::Cable* cables[5];
    for (int cable = 0; cable < 5; cable++) cables[cable] = new physics::Cable();

    // +------------------------ DECLARE OBJECT WORLD ------------------------+
    ObjectWorld terra;

    // +------------------------ OBJECT INITIALIZATIONS ------------------------+
    /*float initialForce = -70.0f;
    float particle_radius = 40.0f;
    float particle_gap = 85.0f;
    float cableLength = 300.0f;*/

    for (int i = 0; i<5; i++)
    {
        spheres[i]->setObjPos(particle_gap * (i-2), 0, 0.0f);
        spheres[i]->setMass(500.0f);
        spheres[i]->setRadius(particle_radius);

        anchors[i]->setObjPos(particle_gap * (i - 2), cableLength, 0.0f);
        anchors[i]->setRadius(5.0f);
        cables[i]->particles[0] = spheres[i]->getParticleAddress();
        cables[i]->particles[1] = anchors[i]->getParticleAddress();
        cables[i]->length = cableLength;
    }

    //Initial force that moves the left most sphere 
    /*spheres[0]->setObjVel(initialForce, 0.0f, 0.0f);*/
    
    // +------------------------ PUSH OBJECTS INTO OBJECT WORLD ------------------------+
    for (Object* obj : spheres) terra.AddObject(obj, true);
    for (Object* obj : anchors) terra.AddObject(obj, false);
    for (int n = 0; n < 5; n++) terra.Links.push_back(cables[n]);

    // +------------------------ TIME ------------------------+
    //Initialize the clock and variables
    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    // Enable Depth
    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // +------------------------ DELTA TIME ------------------------+
        //Get Current time
        curr_time = clock::now();
        //Check the duration in between the last iteration
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time);
        //Set the pre with the current for the next iteration
        prev_time = curr_time;

        //Add the duration since the last iteration
        //to the time since our last "frame"
        curr_ns += dur;


        // +------------------------ UPDATES ------------------------+
        if (curr_ns >= timestep) // Fixed Updates
        {
            //Convert ns to ms
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            //std::cout << "FixedUpdate MS: " << (float)ms.count() << std::endl;
            //Reset
            curr_ns -= curr_ns;

            // If the user pauses the program, it should not update the worlds
            if (!isPaused)
            {
                float deltaTime = (float)ms.count() / 1000;
                terra.Update(deltaTime);
            }
        }

        // +------------------------ GET USER INPUT ------------------------+
        glfwSetKeyCallback(window, key_callback);   //  Pause
        generalCamera.getUserInput(window);         //  Camera Controls

        // +------------------------ UPDATES ------------------------+
        generalCamera.update();

        // +------------------------ RENDER ------------------------+
        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render all objects
        terra.Render(shader, generalCamera);

        // +--------------------------------------------------------------------------------------------------------------------------+

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    //Delete Spheres, Anchors, and Cables
    for (Object* obj : spheres) delete obj;
    for (Object* obj : anchors) delete obj;

    for (physics::Cable* c : cables) delete c;

    glfwTerminate();
    return 0;
}