#ifndef FORCEGENERATOR_H
#define FORCEGENERATOR_H

#include "Particle.h"

namespace physics
{

	class ForceGenerator
	{
	public:
		virtual void updateForce(physics::P6Particle* p, float time);
	};
	
}

#endif