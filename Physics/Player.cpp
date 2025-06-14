#include "Player.h"

void Player::update(float dTime)
{
	particle.update(dTime);
	//Set the position of the model to the position of its particle component
	model3D->modelPos = vec3(particle.pos);

	race_time += dTime;
};