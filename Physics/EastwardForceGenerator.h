#ifndef EASTWARDFORCEGENERATOR_H
#define EASTWARDFORCEGENERATOR_H

#include "ForceGenerator.h"
#include "Particle.h"

namespace physics
{
	class EastwardForceGenerator : public ForceGenerator
	{
	protected:
		float force;
	public:
		EastwardForceGenerator() { force = 0.0f; };
		EastwardForceGenerator(float _force) :force(_force) {};
		virtual void updateForce(physics::P6Particle* p, float time) override;
	};

}

#endif