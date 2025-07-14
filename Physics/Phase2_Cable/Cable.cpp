#include "Cable.h"

namespace physics
{
	CradleParticleContact* physics::Cable::GetContact()
	{
		float currLen = CurrentLength();

		if (currLen == length)
		{
			return nullptr;
		}

		//Create a contact
		CradleParticleContact* ret = new CradleParticleContact();

		// Assign the two particles
		ret->particles[0] = particles[0];
		ret->particles[1] = particles[1];

		//Get the direction of collision
		Vector dir = particles[1]->pos - particles[0]->pos;
		dir.normalize();

		if (currLen > length)
		{
			ret->contactNormal = dir;
			ret->depth = currLen - length;
		}
		//else
		//{
		//	ret->contactNormal = dir;
		//	ret->depth = length - currLen;
		//}

		ret->restitution = restitution;

		return ret;
	}
}
