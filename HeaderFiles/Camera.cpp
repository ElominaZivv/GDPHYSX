#include "Camera.h"

Camera::Camera(float newWindowWidth, float newWindowHeight, float _fov)
{
	windowWidth = newWindowWidth;
	windowHeight = newWindowHeight;
	this->fov = _fov;

	worldUp = normalize(vec3(0.f, 1.f, 0.f));
	position = vec3(0.f, 0.f, 0.f);
	cameraGaze = vec3(0,-700.f, 0.f); // Gaze is where the camera is looking
	viewMatrix = lookAt(position, cameraGaze, worldUp);
	distance = 1300;
	//Orthographic Camera by default
	perspectiveCam = false;
	orthoCam = true;

	//Orthographic by default
	projectionMatrix = ortho(
		-fov,
		fov,
		-fov,
		fov,
		zNear,
		zFar
	);

	projectionMatrix = perspective(
		radians(fov),				//This is your FOV
		windowHeight / windowWidth, //Aspect ratio
		zNear,                      //z-Near, should never be <= 0
		zFar						//z-Far   
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
			zNear, zFar
		);
	}
	if (perspectiveCam)
	{
		projectionMatrix = perspective(
			radians(60.f),				//This is your FOV
			windowHeight / windowWidth, //Aspect ratio
			zNear,                      //z-Near, should never be <= 0
			zFar						//z-Far   
		);
	}

	/*
	
	physics::Vector dir = physics::Vector(sin(position.x), tan(position.y), cos(position.x)).dir() * distance;
	//position = vec3(sin(position.x) + dir.x, tan(position.y) + dir.y, cos(position.x) + dir.z);

	viewMatrix = lookAt(vec3(sin(position.x) + dir.x, tan(position.y) + dir.y, cos(position.x) + dir.z), cameraGaze, worldUp);
	//viewMatrix = lookAt(vec3(position.x), cameraGaze, worldUp);
	
	*/

	viewMatrix = lookAt(position, cameraGaze, worldUp);
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
		//position.x -= .01f * distance;
		thetaX -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		//position.x += .01f * distance;
		thetaX += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_W)) {
		//position.y += .01f * distance;
		if (thetaY > -89.0f) thetaY -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		//position.y -= .01f * distance;
		if (thetaY < 89.0f) thetaY += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_Q)) {
		distance += 1.f;
	}
	if (glfwGetKey(window, GLFW_KEY_E)) {
		distance -= 1.f;
	}

	// 3rd person camera movement (Thin Matrix, 2024)
	float groundDist = distance * cos(radians(thetaY));
	position = vec3(
		cameraGaze.x + (groundDist * sin(radians(thetaX))),
		cameraGaze.y + (-distance * sin(radians(thetaY))),
		cameraGaze.z + (groundDist * cos(radians(thetaX)))
	);
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


