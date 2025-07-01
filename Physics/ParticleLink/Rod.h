#ifndef ROD_H
#define ROD_H

#include "ParticleLink.h"

namespace physics
{
	class Rod : public ParticleLink
	{
	public:
		//Length of the rod
		float length = 1;

		//Restitution
		float restitution = 0;

		ParticleContact* GetContact() override;

	};
}

#endif