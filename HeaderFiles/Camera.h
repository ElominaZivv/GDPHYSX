#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class Camera
{
private:
	bool perspectiveCam, orthoCam;
	float windowWidth, windowHeight;
	vec3 worldUp, position, cameraGaze;
	mat4 cameraPosMatrix, cameraOrientation, viewMatrix, projectionMatrix;

public:
	Camera(float newWindowWidth, float newWindowHeight, float fov);
	void update();
	void toggleCam();
	mat4 getViewMatrix();
	mat4 getProjectionMatrix();

};

#endif