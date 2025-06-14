#ifndef GRAVITYFORCEGENERATOR_H
#define GRAVITYFORCEGENERATOR_H

#include "ForceGenerator.h"

namespace physics
{
	class GravityForceGenerator : public ForceGenerator
	{
	private:
		physics::Vector gravity = physics::Vector(0, -9.8f, 0);

	public:
		GravityForceGenerator(const physics::Vector gravity) : gravity(gravity) {}
		void updateForce(physics::P6Particle* particle, float dTime) override;
	};

}

#endif