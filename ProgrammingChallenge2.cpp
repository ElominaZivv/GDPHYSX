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
#include "Physics/Phase2_Cable/CradleParticleContact.h"
#include "Physics/ParticleLink/AnchoredRod.h"
#include "Physics/ParticleLink/AnchoredRodParticleContact.h"
#include "Physics/Line.h"

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
bool isSpun = false;
bool isStopped = false;

// +------------------------+ DECLARE OBJECT WORLD +------------------------+
ObjectWorld terra;

// +------------------------+ USER INPUTS +------------------------+
Object* spheres[5];
float initialForce = 999.999f;
float particle_radius = 40.0f;
float particle_gap = 85.0f;
float cableLength = 300.0f;
float gravity = -9.8f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && !isSpun)
    {
        spheres[0]->setObjVel(initialForce, 0.0f, 0.0f);
        isSpun = true;
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && isStopped)
    {
        terra.findTopMostParticle();
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800,800, "Elomina", NULL, NULL);
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
    /*
    printf("Particle Radius : ");
    cin >> particle_radius;
    printf("Particle Gap : ");
    cin >> particle_gap;
    printf("Cable Length : ");
    cin >> cableLength;
    printf("Gravity : ");
    cin >> gravity;
    */

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

    // +------------------------ PROGRAMMING CHALLENGE 2 ------------------------+
    // +------------------------ DECLARE OBJECTS ------------------------+
    // Spheres
    /*Object* spheres[5];*/
    for (int sphere = 0; sphere < 5; sphere++) spheres[sphere] = new Object(sphereVAO);
    
    // Anchor
    Object* center;
    center = new Object(sphereVAO);

    //Cables
    physics::AnchoredRod* spokes[5];
    for (int spoke = 0; spoke < 5; spoke++) spokes[spoke] = new physics::AnchoredRod();

    //Rod
    physics::Rod* rods[5];
    for (int rod = 0; rod < 5; rod++) rods[rod] = new physics::Rod();

    //Lines
    vector<Line*> lines;

    float particle_radius = 50.0;
    float cableLength = 400.0f;
    float gravity = 0.0f;
    // +------------------------ DECLARE OBJECT WORLD ------------------------+
    terra.gravity = physics::Vector(0, gravity, 0);

    // +------------------------ OBJECT INITIALIZATIONS ------------------------+
    // Set Sphere Prize
    spheres[0]->prize = " +15 Speed     (blue)";
    spheres[1]->prize = " +20 Stamina   (ourple)";
    spheres[2]->prize = " +25 Power     (pink)";
    spheres[3]->prize = " +30 Guts      (yellow)";
    spheres[4]->prize = " +35 Wit       (green)";

    // Set Sphere Position
    spheres[0]->setObjPos(0,1.f * cableLength, 0.0f);
    spheres[1]->setObjPos(0.95542f * cableLength, 0.29524f * cableLength, 0.0f);
    spheres[2]->setObjPos(0.70761f * cableLength, -0.70661f * cableLength, 0.0f);
    spheres[3]->setObjPos(-0.70761f * cableLength, -0.70661f * cableLength, 0.0f);
    spheres[4]->setObjPos(-0.95542f * cableLength, 0.29524f * cableLength, 0.0f);

    //Set Sphere Color
    spheres[0]->setColor(45, 0, 255);
    spheres[1]->setColor(170, 0, 255);
    spheres[2]->setColor(255, 0, 135);
    spheres[3]->setColor(255, 250, 0);
    spheres[4]->setColor(0,255,0);

    // Set Center Position
    center->setObjPos(0.0f, 0.0f, 0.0f);
    center->setDamping(0.0f);
    center->setColor(1,1,1);
    center->setRadius(250);

    for (int i = 0; i<5; i++)
    {
        spheres[i]->setMass(50.0f);
        spheres[i]->setRadius(particle_radius);
        spheres[i]->setRestitution(0.9);

        // Rod
        rods[i]->particles[0] = spheres[i]->getParticleAddress();
        rods[i]->particles[1] = spheres[(i+1)%5]->getParticleAddress();
        // Rod length is approximately 1.0320432249 * cableLength
        rods[i]->length = 1.0320432249 * cableLength;

        spokes[i]->particles[0] = spheres[i]->getParticleAddress();
        spokes[i]->particles[1] = center->getParticleAddress();
        spokes[i]->length = cableLength;
    }

    for (int i = 0; i < 5; i++) {
        Line* line = new Line(center->getParticleAddress(), spheres[i]->getParticleAddress());
        lines.push_back(line);
    }
    
    // +------------------------ PUSH OBJECTS INTO OBJECT WORLD ------------------------+
    for (Object* obj : spheres) terra.AddObject(obj, true);
    terra.AddObject(center, false);
    for (int n = 0; n < 5; n++) terra.Links.push_back(spokes[n]);
    for (int n = 0; n < 5; n++) terra.Links.push_back(rods[n]);

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
        curr_ns += dur*5;


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
                isStopped=terra.checkStop(isSpun);
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
    delete center;

    for (physics::AnchoredRod* ar : spokes) delete ar;
    for (physics::Rod* r : rods) delete r;
    for (Line* line : lines) delete line;

    glfwTerminate();
    return 0;
}

/*
5 spheres
50 kg sphere
Roullete
All spheres are different colors
outcome per sphere 
result should be on the top most

Before simulation, print outcomes for each sphere (all unique)
It should stop (dampening, or space to decelerate)
Print result on the terminal window

All particles should be visible
Build it as release

submit as a ZIP
*/