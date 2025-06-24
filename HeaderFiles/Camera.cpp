#include "Camera.h"

Camera::Camera(float newWindowWidth, float newWindowHeight, float _fov)
{
	windowWidth = newWindowWidth;
	windowHeight = newWindowHeight;
	this->fov = _fov;

	worldUp = normalize(vec3(0.f, 1.f, 0.f));
	position = vec3(0.f, 0.f, 0);
	cameraGaze = vec3(0, 0.f, 0.f); // Gaze is where the camera is looking.=
	viewMatrix = lookAt(position, cameraGaze, worldUp);
	//Orthographic Camera by default
	perspectiveCam = false;
	orthoCam = true;

	//Orthographic by default
	projectionMatrix = ortho(
		-fov,
		fov,
		-fov,
		fov,
		-fov,
		fov
	);
}

void Camera::update()
{
	//Update ProjectionMatrix based on the toggled Camera Mode (Perspective Or Ortho)
	if (orthoCam)
	{
		projectionMatrix = ortho(
			-fov, fov,
			-fov, fov,
			-fov, fov
		);
	}
	if (perspectiveCam)
	{
		projectionMatrix = perspective(
			radians(fov),				//This is your FOV
			windowHeight / windowWidth, //Aspect ratio
			zNear,                      //z-Near, should never be <= 0
			zFar						//z-Far   
		);
	}

	viewMatrix = lookAt(vec3(sin(position.x), tan(clamp(position.y, -1.f, 1.f)), cos(position.x)), cameraGaze, worldUp);
}

void Camera::getUserInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		orthoCam = true;
		perspectiveCam = false;
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		orthoCam = false;
		perspectiveCam = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		position.x -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		position.x += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		if (position.y < 1.f) position.y += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_W)) {
		if (position.y > -1.f) position.y -= 0.01f;
	}
}

void Camera::toggleCam()
{
	perspectiveCam = -perspectiveCam;
	orthoCam = -orthoCam;
}

mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}


