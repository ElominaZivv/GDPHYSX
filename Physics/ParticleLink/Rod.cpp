#include "Rod.h"

namespace physics
{
	ParticleContact* physics::Rod::GetContact()
	{
		float currLen = CurrentLength();

		if (currLen == length)
		{
			return nullptr;
		}

		//Create a contact
		ParticleContact* ret = new ParticleContact();

		// Assign the two particles
		ret->particles[0] = particles[0];
		ret->particles[1] = particles[1];

		//Get the direction of collision
		Vector dir = particles[1]->pos - particles[0]->pos;
		dir.normalize();

		if (currLen > length)
		{
			ret->contactNormal = dir;
			//ret->depth = currLen - length; //Implement depth later
		}
		else
		{
			ret->contactNormal = dir;
			//ret->depth = length - currLen; //Implement depth later
		}

		ret->restitution = restitution;

		return ret;
	}
}
