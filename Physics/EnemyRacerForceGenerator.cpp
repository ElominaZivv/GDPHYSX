#include "EnemyRacerForceGenerator.h"

namespace physics
{


	EnemyRacerForceGenerator ::EnemyRacerForceGenerator(float lowerMult, float UpperMult, float _boostThreshold) : EastwardForceGenerator(rngFloat(20.0, 30.0))
	{
		boost = rngFloat(lowerMult, UpperMult);
		boostThreshold = _boostThreshold;
	}

	void EnemyRacerForceGenerator::updateForce(physics::P6Particle* particle, float dTime)
	{
		if (particle->pos.x <= boostThreshold)
		{
			particle->addForce(Vector(1, 0, 0) * force);
		}
		else
		{
			particle->addForce(Vector(1, 0, 0) * force * boost);
		}
	}

}