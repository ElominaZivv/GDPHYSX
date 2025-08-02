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
bool spin = false;
bool stop = false;

// +------------------------+ INITIALS +------------------------+
ObjectWorld terra;
Object* spheres[5];
float initialForce = 0;
float particle_radius = 50.f;
float cableLength = 300.0f;
const double PI = 3.14159265358979323846;
float orientation = PI/2;
string prize;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && !spin) {
        spheres[0]->setObjVel(initialForce, 0.0f, 0.0f);
        spin = true;
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && stop && spin) {
        prize = terra.checkPrize();
        cout << "You Won " << prize;
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800,800, "PC02-Chen", NULL, NULL);
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
    printf("Force : ");
    cin >> initialForce;

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
    Object* anchor;
    anchor = new Object(sphereVAO);

    //Cables
    physics::Cable* cables[5];
    for (int cable = 0; cable < 5; cable++) cables[cable] = new physics::Cable();

    //Lines
    vector<Line*> lines;

    //Rods
    physics::Rod* rods[5];
    for (int rod = 0; rod < 5; rod++) rods[rod] = new physics::Rod();

    
    // +------------------------ OBJECT INITIALIZATIONS ------------------------+

    terra.gravity = physics::Vector(0, 0, 0);
    
    anchor->setObjPos(0, 0, 0);
    anchor->setColor(1, 1, 1);

    for (int i = 0; i<5; i++)
    {

        spheres[i]->setMass(50.0f);
        spheres[i]->setRadius(particle_radius);
        spheres[i]->setRestitution(0.9);
        // Formula used https://math.stackexchange.com/questions/1990504/how-to-find-the-coordinates-of-the-vertices-of-a-pentagon-centered-at-the-origin
        float angle = orientation + i * 2 * PI / 5;
        spheres[i]->setObjPos(cableLength * cos(angle), cableLength * sin(angle), 0.f);

        cables[i]->particles[0] = spheres[i]->getParticleAddress();
        cables[i]->particles[1] = anchor->getParticleAddress();
        cables[i]->length = cableLength;

       
        rods[i]->particles[0] = spheres[i]->getParticleAddress();
        rods[i]->particles[1] = spheres[(i+1)%5]->getParticleAddress();
        rods[i]->length = 352.67; //length from distance formula
    }

    for (int i = 0; i < 5; i++) {
        Line* line = new Line(anchor, spheres[i]->getParticleAddress());
        lines.push_back(line);
    }

    spheres[0]->setColor(1, 0, 0);
    spheres[0]->setPrize("Red: Red Shoes");

    spheres[1]->setColor(2, 1, 0);
    spheres[1]->setPrize("Orange: Coins");

    spheres[2]->setColor(1, 1, 0);
    spheres[2]->setPrize("Yellow: Yellow Shoes");

    spheres[3]->setColor(0, 1, 0);
    spheres[3]->setPrize("Green: Green Shoes");

    spheres[4]->setColor(0, 0, 1);
    spheres[4]->setPrize("Blue: Blue Shoes");

    cout << "____________________" << endl << "Prizes" << endl << "____________________" << endl;
    cout << "Red: Red Shoes" << endl << "Orange: Coins" << endl << "Yellow: Yellow Shoes" << endl << "Green: Green Shoes" << endl << "Blue: Blue Shoes" << endl << "____________________" << endl;
    // +------------------------ PUSH OBJECTS INTO OBJECT WORLD ------------------------+
    for (Object* obj : spheres) terra.AddObject(obj, true);
    terra.AddObject(anchor, false);
    for (int n = 0; n < 5; n++) terra.Links.push_back(cables[n]);
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
        glfwSetKeyCallback(window, key_callback);   //  Shove
        generalCamera.getUserInput(window);         //  Camera Controls

        // +------------------------ UPDATES ------------------------+
        generalCamera.update();
        for (int i = 0; i < lines.size(); i++) {
            lines[i]->update();
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            terra.addDamping();
        }

        stop = terra.checkStop(spin);
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
    delete anchor;
    for (physics::Cable* c : cables) delete c;
    for (Line* line : lines) delete line;
    for (physics::Rod* r : rods) delete r;

    glfwTerminate();
    return 0;
}