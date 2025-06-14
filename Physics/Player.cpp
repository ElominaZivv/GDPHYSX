#include "Player.h"

void Player::update(float dTime)
{
	particle.update(dTime);
	//Set the position of the model to the position of its particle component
	model3D->modelPos = vec3(particle.pos);

	race_time += dTime;
};

void Player::getUserInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) keyDown++;

	if (keyDown>=1 && keyDown<=35)particle.addForce(physics::Vector(10, 0, 0));
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) keyDown=0;

	std::cout << keyDown << std::endl;
	
	
}