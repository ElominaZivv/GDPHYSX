#include "ForceRegistry.h"

namespace physics
{

	void ForceRegistry::add(P6Particle* particle, ForceGenerator* generate)
	{
		ParticleForceRegistry toAdd;
		toAdd.particle = particle;
		toAdd.generator = generate;
		registry.push_back(toAdd);
	}

	void ForceRegistry::remove(P6Particle* particle, ForceGenerator* generate)
	{
		registry.remove_if(
			[particle, generate](ParticleForceRegistry reg) {
				return reg.particle == particle && reg.generator == generate;
			}
		);
	}

	void ForceRegistry::clear()
	{
		registry.clear();
	}

	void ForceRegistry::updateForces(float dTime)
	{
		for (list<ParticleForceRegistry>::iterator i = registry.begin();
			i != registry.end();
			i++)
		{
			i->generator->updateForce(i->particle, dTime);
		}
	}

}