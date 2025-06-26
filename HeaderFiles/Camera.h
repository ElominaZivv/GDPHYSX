#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Physics/Vector.h"

using namespace std;
using namespace glm;

class Camera
{
private:
	bool perspectiveCam, orthoCam;
	float windowWidth, windowHeight;
	vec3 worldUp, position, cameraGaze;
	mat4 cameraPosMatrix, cameraOrientation, viewMatrix, projectionMatrix;
	float distance;
	float thetaX=0.0f, thetaY=0.0f;
	float fov;
	float zNear = 0.1f;
	float zFar = 100000.0f;

public:
	Camera(float newWindowWidth, float newWindowHeight, float _fov);
	void update();
	void getUserInput(GLFWwindow* window);
	void toggleCam();
	mat4 getViewMatrix();
	mat4 getProjectionMatrix();

};

#endif