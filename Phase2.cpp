#include <iostream>
#include <list>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

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
#include "Physics/Phase2_Cable/CradleParticleContact.h"
#include "Physics/Line.h"

//Springs
#include "Physics/Spring/AnchoredSpring.h"
#include "Physics/Spring/ParticleSpring.h"

using namespace std;
using namespace glm;

const int PARTICLE_COUNT = 5;

// +------------------------+ TIME LIBRARIES +------------------------+
#include<chrono>
using namespace std::chrono_literals;
// We will call this our time between "frames"
constexpr::std::chrono::nanoseconds timestep(16ms);


// +------------------------+ DEVELOPER STUFFS +------------------------+
bool isPaused = false;

// +------------------------+ USER INPUTS +------------------------+
Object* spheres[PARTICLE_COUNT];
float initialForce = -70.0f;
float particle_radius = 40.0f;
float particle_gap = 85.0f;
float cableLength = 300.0f;
float gravity = 0;

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
    window = glfwCreateWindow(800,800, "Phase2_Grouping2_Chen-Elomina-Naranjo", NULL, NULL);
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
    //printf("Initial Force : ");
    //cin >> initialForce;
    //printf("Particle Radius : ");
    //cin >> particle_radius;
    //printf("Particle Gap : ");
    //cin >> particle_gap;
    //printf("Cable Length : ");
    //cin >> cableLength;
    //printf("Gravity : ");
    //cin >> gravity;

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
    for (int sphere = 0; sphere < PARTICLE_COUNT; sphere++) spheres[sphere] = new Object(sphereVAO);
    
    // Anchor
    Object* anchors[PARTICLE_COUNT];
    for (int a = 0; a < PARTICLE_COUNT; a++) anchors[a] = new Object(sphereVAO);

    //Cables
    physics::Cable* cables[PARTICLE_COUNT];
    for (int cable = 0; cable < PARTICLE_COUNT; cable++) cables[cable] = new physics::Cable();

    // Rods
    physics::Rod* rods[PARTICLE_COUNT];
    for (int i = 0; i < PARTICLE_COUNT; i++) rods[i] = new physics::Rod();

    //Lines
    vector<Line*> lines;

    // +------------------------ DECLARE OBJECT WORLD ------------------------+
    ObjectWorld terra;
    terra.gravity = physics::Vector(0, gravity, 0);

    // +------------------------ OBJECT INITIALIZATIONS ------------------------+
    float initialForce = -70.0f;
    float particle_radius = 40.0f;
    float particle_gap = 85.0f;
    float cableLength = 300.0f;

    spheres[0]->setObjPos(cableLength * cos(radians(270.f)), cableLength * sin(radians(270.f)), 0);
    spheres[1]->setObjPos(cableLength * cos(radians(342.f)), cableLength * sin(radians(342.f)), 0);
    spheres[2]->setObjPos(cableLength * cos(radians(54.f)), cableLength * sin(radians(54.f)), 0);
    spheres[3]->setObjPos(cableLength * cos(radians(126.f)), cableLength * sin(radians(126.f)), 0);
    spheres[4]->setObjPos(cableLength * cos(radians(198.f)), cableLength * sin(radians(198.f)), 0);

    for (int i = 0; i<PARTICLE_COUNT; i++)
    {
        /*spheres[i]->setObjPos(particle_gap * (i-2), 0, 0.0f);*/
        spheres[i]->setMass(50.0f);
        spheres[i]->setRadius(particle_radius);
        spheres[i]->setRestitution(1);

        rods[i]->particles[0] = spheres[i]->getParticleAddress();
        rods[i]->particles[1] = spheres[(i+1) % PARTICLE_COUNT]->getParticleAddress();
        rods[i]->length = cableLength * 1.2084014531188;

        anchors[i]->setObjPos(0, 0, 0.0f);
        anchors[i]->setRadius(5.0f);
        cables[i]->particles[0] = spheres[i]->getParticleAddress();
        cables[i]->particles[1] = anchors[i]->getParticleAddress();
        cables[i]->length = cableLength;
    }

    //for (int i = 0; i < 5; i++) {
    //    Line* line = new Line(anchors[i], spheres[i]->getParticleAddress());
    //    lines.push_back(line);
    //}
    
    // +------------------------ PUSH OBJECTS INTO OBJECT WORLD ------------------------+
    for (Object* obj : spheres) terra.AddObject(obj, true);
    for (Object* obj : anchors) terra.AddObject(obj, false);
    for (int n = 0; n < PARTICLE_COUNT; n++) terra.Links.push_back(rods[n]);
    for (int n = 0; n < PARTICLE_COUNT; n++) terra.Links.push_back(cables[n]);

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
        curr_ns += dur*8;


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
        glfwSetKeyCallback(window, key_callback);   //  Shove
        generalCamera.getUserInput(window);         //  Camera Controls

        // +------------------------ UPDATES ------------------------+
        generalCamera.update();
        for (int i = 0; i < lines.size(); i++) {
            lines[i]->update();
        }

        // +------------------------ RENDER ------------------------+
        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render all objects
        terra.Render(shader, generalCamera);
        for (auto& line : lines) {
            line->draw(shader, generalCamera);
        }

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
    for (Line* line : lines) delete line;

    glfwTerminate();
    return 0;
}