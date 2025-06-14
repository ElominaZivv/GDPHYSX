#ifndef ENEMYRACERFORCEGENERATOR_H
#define ENEMYRACERFORCEGENERATOR_H

#include "EastwardForceGenerator.h"
#include "Particle.h"

namespace physics
{

	class EnemyRacerForceGenerator : public EastwardForceGenerator
	{
	private:
		float boostedForce;
		float boostThreshold;
	public:
		EnemyRacerForceGenerator() {}
		EnemyRacerForceGenerator(float _force, float _boostedForce, float _boostThreshold) : EastwardForceGenerator(_force), boostedForce(_boostedForce), boostThreshold(_boostThreshold) {};
		void updateForce(physics::P6Particle* p, float time) override;
	};

}

#endif