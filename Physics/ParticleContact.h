#ifndef PARTICLECONTACT_H
#define PARTICLECONTACT_H

#include "Particle.h"

namespace physics
{
	class ParticleContact
	{
	public:
		P6Particle* particles[2];
		float separatingSpeed=0;
		float restitution;
		Vector contactNormal;
		void Resolve(float time);
		float RetrieveSeparatingSpeed();

	protected:
		float GetSeparatingSpeed();
		void ResolveVelocity(float time);
	};
}


#endif