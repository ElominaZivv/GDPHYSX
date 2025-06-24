#include <iostream>
#include <vector>
#include <random>
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

using namespace std;
using namespace glm;


// +------------------------+ TIME LIBRARIES +------------------------+
#include<chrono>
using namespace std::chrono_literals;
// We will call this our time between "frames"
constexpr::std::chrono::nanoseconds timestep(16ms);

bool isPaused = false;

// +------------------------+ USER INPUTS +------------------------+
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) isPaused = !isPaused;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800,800, "Phase1_Grouping2_Chen-Elomina-Naranjo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    // +------------------------ SET THE SEED OF THE RAND() ------------------------+
    srand(time(0));

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

    // +------------------------ DECLARE OBJECTS ------------------------+

        // things i changed

    random_device random;
    uniform_real_distribution<float> force(-10000.f, 10000.f);
    float minimumAmplitude = 5000.f;
    int maxSparks;
    cout << "Set Amount Of Sparks: ";
    cin >> maxSparks;

    vector<Object*> spheres;

    for (int i = 0; i < maxSparks; i++) {
        Object *newSphere = new Object(sphereVAO);
        newSphere->setObjPos(0, -700, 0);
        spheres.push_back(newSphere);
    }
    //Object sphere1(sphereVAO);
    //Object sphere2(sphereVAO);

    // +------------------------ DECLARE OBJECT WORLD ------------------------+
    ObjectWorld terra;

    // +------------------------ OBJECT INITIALIZATIONS ------------------------+
    // 
    //things i changed
    //sphere1.setObjPos(-50.0, 0.0, 0.0);

    //sphere2.setObjPos(50.0, 0.0, 0.0);

    // +------------------------ FORCE GENERATORS ------------------------+
        
    //Ice
    //physics::DragForceGenerator IceDrag = physics::DragForceGenerator(0.14f, 0.1f);

    // Rubber On Concrete (race car wheels on race track)                           1.0f    0.8f
    // physics::DragForceGenerator rubberOnConcrete = physics::DragForceGenerator(  1.0f,   0.8f);
        
    // +------------------------ PUSH OBJECTS INTO OBJECT WORLD ------------------------+
    //terra.AddObject(&sphere1);
    //terra.AddObject(&sphere2);
    for (int i = 0; i < spheres.size(); i++) {
        terra.AddObject(spheres[i]);
        /*terra.registry.add(spheres[i]->addForce(force(random)));*/
    }

    // +------------------------ TIME ------------------------+
    //Initialize the clock and variables
    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

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

        // +------------------------ GET USER INPUT ------------------------+
        glfwSetKeyCallback(window, key_callback);   //  Pause
        generalCamera.getUserInput(window);         //  Camera Controls

        // +------------------------ UPDATES ------------------------+
        generalCamera.update();
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
        

        // +------------------------ RENDER ------------------------+
        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Render all objects
        terra.Render(shader, generalCamera);

        // +--------------------------------------------------------------------------------------------------------------------------+

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}