#include "EnemyRacerForceGenerator.h"

namespace physics
{

	void EnemyRacerForceGenerator::updateForce(physics::P6Particle* particle, float dTime)
	{
		if (particle->pos.x <= boostThreshold)
		{
			particle->addForce(Vector(1, 0, 0) * force);
		}
		else
		{
			particle->addForce(Vector(1, 0, 0) * boostedForce);
		}
	}

}