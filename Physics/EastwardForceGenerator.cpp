#include "EastwardForceGenerator.h"

namespace physics
{

	void EastwardForceGenerator::updateForce(physics::P6Particle* particle, float dTime)
	{
		particle->addForce(Vector(1,0,0)*force);
	}

}