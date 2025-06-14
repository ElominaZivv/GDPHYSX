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
#include "Physics/Player.h"

//My Physics Files
#include "Physics/ForceGenerator.h"
#include "Physics/ForceRegistry.h"
#include "Physics/GravityForceGenerator.h"
#include "Physics/DragForceGenerator.h"
#include "Physics/EastwardForceGenerator.h"
#include "Physics/EnemyRacerForceGenerator.h"
#include "Physics/RNG.h"

#include <cstdlib>
#include <time.h>

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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) isPaused = !isPaused;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1100,500, "PC01 - Elomina, Zivv", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, -300, 1100, 1100);

    // +------------------------ TIME ------------------------+
    //Initialize the clock and variables
    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);
    //rand() seed
    srand(time(0));

    // +------------------------ GET USER INPUT ------------------------+
    glfwSetKeyCallback(window, key_callback);

    // +------------------------ DECLARE SHADERS ------------------------+
    Shader shader("Shaders/solidColorShader.vert", "Shaders/solidColorShader.frag");

    // +------------------------ DECLARE VAOs ------------------------+
    //Smart pointer that is shared and not copied when passed into the constructor of a class.
    //When out of scope, it will delete itself
    auto sphereVAO = make_shared<VAO>("3D/sphere.obj");

    // +------------------------ DECLARE CAMERA ------------------------+
    float windowWidth = 1100.0f;
    float windowHeight = 1100.0f;
    float fov = 550.f;
    Camera orthoCam(windowWidth, windowHeight, fov);
    
    // +------------------------ DECLARE OBJECTS ------------------------+
    Player player(  "HaksuneTwa", sphereVAO);
    Object sphere1( "Gengavitis", sphereVAO);
    Object sphere2( "Adventring", sphereVAO);
    Object sphere3( "BeggaMalls", sphereVAO);
    player.setSize(5.0f);
    sphere1.setSize(5.0f);
    sphere2.setSize(5.0f);
    sphere3.setSize(5.0f);

    // +------------------------ DECLARE OBJECT WORLD ------------------------+
    physics::ObjectWorld terra;
    terra.finishLine = 500.0f;

    // +------------------------ OBJECT INITIALIZATIONS ------------------------+
    player.setObjPos(-500.0f, fov / 3, 0.0);
    sphere1.setObjPos(-500.0f, fov / 9, 0.0);
    sphere2.setObjPos(-500.0f, fov / -9, 0.0);
    sphere3.setObjPos(-500.0f, fov / -3, 0.0);

    player.setColor(0.5, 0.0, 1.0);

    // +------------------------ DRAG FORCE GENERATORS ------------------------+
        
    //Ice
    physics::DragForceGenerator IceDrag = physics::DragForceGenerator(0.14f, 0.1f);

    // Rubber On Concrete (race car wheels on race track)                         1.0f    0.8f
     physics::DragForceGenerator rubberOnConcrete = physics::DragForceGenerator(  1.0f,   0.8f);

    // Movement Force Generator
    physics::EnemyRacerForceGenerator enemyForce1 = physics::EnemyRacerForceGenerator(1.1, 8.0, fov * 0.1f);
    physics::EnemyRacerForceGenerator enemyForce2 = physics::EnemyRacerForceGenerator(1.1, 8.0, fov * 0.1f);
    physics::EnemyRacerForceGenerator enemyForce3 = physics::EnemyRacerForceGenerator(1.1, 8.0, fov * 0.1f);
        
    // +------------------------ PUSH OBJECTS INTO OBJECT WORLD ------------------------+
    terra.AddObject(&player);
    terra.AddObject(&sphere1);
    terra.AddObject(&sphere2);
    terra.AddObject(&sphere3);
    
    // Add the Ice Drag to the force generator registry of ObjectWorld, Terra. 

        //ICE WORLD
    terra.registry.add(player.getParticleAddress(), &IceDrag);
    terra.registry.add(sphere1.getParticleAddress(), &IceDrag);
    terra.registry.add(sphere2.getParticleAddress(), &IceDrag);
    terra.registry.add(sphere3.getParticleAddress(), &IceDrag);
    

    /*
        //RUBBER ON CONCRETE WORLD
    terra.registry.add(player.getParticleAddress(), &rubberOnConcrete);
    terra.registry.add(sphere1.getParticleAddress(), &rubberOnConcrete);
    terra.registry.add(sphere2.getParticleAddress(), &rubberOnConcrete);
    terra.registry.add(sphere3.getParticleAddress(), &rubberOnConcrete);
    */

    terra.registry.add(sphere1.getParticleAddress(), &enemyForce1);
    terra.registry.add(sphere2.getParticleAddress(), &enemyForce2);
    terra.registry.add(sphere3.getParticleAddress(), &enemyForce3);

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
        // GetUserInput
        player.getUserInput(window);

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
                terra.Update((float)ms.count() / 1000);
            }
        }
        

        // +------------------------ RENDER ------------------------+
        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Render all objects
        terra.Render(shader, orthoCam);

        // +--------------------------------------------------------------------------------------------------------------------------+

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

/*
    May 23, 2025
    Particle / Point Mass
     - No Radius
     - No Size
     - No Rotation

     - Mass
     - Position
     - Velocity
     - Acceleration
     - Aside from mass- everything is a Vector

     Keep everything in scale
     length = meters
     mass = kilogram
     time = seconds
*/