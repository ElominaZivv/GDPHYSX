#include "GravityForceGenerator.h"

namespace physics
{

	void GravityForceGenerator::updateForce(physics::P6Particle* particle, float dTime)
	{
		if (particle->mass == 0) return;
		physics::Vector force = gravity * particle->mass;

		particle->addForce(force);
	}

}
