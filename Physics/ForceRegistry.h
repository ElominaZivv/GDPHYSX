#ifndef FORCEREGISTRY_H
#define FORCEREGISTRY_H

#include "Particle.h"
#include "ForceGenerator.h"
#include <list>

namespace physics
{

	class ForceRegistry
	{
	public:
		void add(P6Particle* particle, physics::ForceGenerator* generate);
		void remove(P6Particle* particle, physics::ForceGenerator* generate);
		void clear();
		void updateForces(float dTime);

	protected:
		struct ParticleForceRegistry
		{
			P6Particle* particle;
			ForceGenerator* generator;
		};
		list<ParticleForceRegistry> registry;
	};

}

#endif