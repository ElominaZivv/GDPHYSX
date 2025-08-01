#include "AnchoredRod.h"

namespace physics
{
	AnchoredRodParticleContact* physics::AnchoredRod::GetContact()
	{
		float currLen = CurrentLength();

		if (currLen == length)
		{
			return nullptr;
		}

		//Create a contact
		AnchoredRodParticleContact* ret = new AnchoredRodParticleContact();

		// Assign the two particles
		ret->particles[0] = particles[0];
		ret->particles[1] = particles[1];

		//Get the direction of collision
		Vector dir = particles[1]->pos - particles[0]->pos;
		dir.normalize();
		//particles[1]->pos.debug();
		//particles[0]->pos.debug();
		//dir.debug();
		//std::cout << std::endl;

		if (currLen > length)
		{
			// Tension
			// During tension when particles are being pulled apart, the contact normal should be positive to compress the particles
			ret->contactNormal = dir;
			ret->depth = currLen - length; //Implement depth later
		}
		else
		{
			// Compression
			// During compression when particles are being squeezed together, the contact normal should be negative to pull them apart
			ret->contactNormal = (dir*(-1));
			ret->depth = length - currLen; //Implement depth later
		}
		ret->restitution = restitution;

		return ret;
	}
}
