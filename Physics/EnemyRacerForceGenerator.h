#ifndef ENEMYRACERFORCEGENERATOR_H
#define ENEMYRACERFORCEGENERATOR_H

#include "EastwardForceGenerator.h"
#include "Particle.h"
#include "RNG.h"

#include <cstdlib>

namespace physics
{

	class EnemyRacerForceGenerator : public EastwardForceGenerator
	{
	private:
		float boost;
		float boostThreshold;
	public:
		EnemyRacerForceGenerator();
		EnemyRacerForceGenerator(float lowerMult, float UpperMult, float _boostThreshold);
		void updateForce(physics::P6Particle* p, float time) override;
	};

}

#endif