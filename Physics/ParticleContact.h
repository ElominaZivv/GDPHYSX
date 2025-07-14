#ifndef PARTICLECONTACT_H
#define PARTICLECONTACT_H

#include "Particle.h"

namespace physics
{
	class ParticleContact
	{
	public:
		P6Particle* particles[2];
		float particleRadii[2];
		float restitution;
		float fSeparatingSpeed;
		Vector contactNormal;
		virtual void Update();
		virtual void Resolve(float time);
		float depth = 0.0;

	protected:
		float GetSeparatingSpeed();
		virtual void ResolveVelocity(float time);
		virtual void ResolveInterpenetration(float time);
	};
}


#endif