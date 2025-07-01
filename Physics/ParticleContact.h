#ifndef PARTICLECONTACT_H
#define PARTICLECONTACT_H

#include "Particle.h"

namespace physics
{
	class ParticleContact
	{
	public:
		P6Particle* particles[2];
		float restitution;
		float fSeparatingSpeed;
		Vector contactNormal;
		void Update();
		void Resolve(float time);
		float depth;

	protected:
		float GetSeparatingSpeed();
		float GetDepth();
		void ResolveVelocity(float time);
		void ResolveInterpenetration(float time);
	};
}


#endif