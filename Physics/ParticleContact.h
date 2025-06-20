#ifndef PARTICLECONTACT_H
#define PARTICLECONTACT_H

#include "Particle.h"

namespace physics
{
	class ParticleContact
	{
	public:
		bool operator<(ParticleContact& p);
		P6Particle* particles[2];
		float separatingSpeed=0;
		float restitution;
		Vector contactNormal;
		void Resolve(float time);

	protected:
		float GetSeparatingSpeed();
		void ResolveVelocity(float time);
	};
}


#endif