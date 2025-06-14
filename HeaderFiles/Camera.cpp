#include "Camera.h"

Camera::Camera(float newWindowWidth, float newWindowHeight, float fov)
{
	windowWidth = newWindowWidth;
	windowHeight = newWindowHeight;

	worldUp = normalize(vec3(0.f, 1.f, 0.f));
	position = vec3(0.f, 0.f, 1.f);
	cameraGaze = vec3(0.f, 0.f, 0.f); // Gaze is where the camera is looking.
	viewMatrix = lookAt(position, cameraGaze, worldUp);
	//Orthographic Camera by default
	perspectiveCam = false;
	orthoCam = true;
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
	cout << "update Camera" << endl;
	//Update ProjectionMatrix based on the toggled Camera Mode (Perspective Or Ortho)
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


