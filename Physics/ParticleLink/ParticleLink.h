#ifndef P6CONTACT_H
#define P6CONTACT_H

#include "../ParticleContact.h"

namespace physics
{
	class ParticleLink
	{
	public:
		//The two particles linked together
		P6Particle* particles[2];
		
		//Get Contact - generate contact / collisions if >< len to our set length
		virtual ParticleContact* GetContact() { return nullptr; }

	protected:
		float CurrentLength();
	};
}

#endif