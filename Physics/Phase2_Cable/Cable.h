#ifndef CABLE_H
#define CABLE_H

#include "../ParticleLink/ParticleLink.h"
#include "CradleParticleContact.h"

namespace physics
{	
	class Cable : public ParticleLink
	{
	public:
		//Length of the rod
		float length = 1;

		//Restitution
		float restitution = 0.0f;

		CradleParticleContact* GetContact() override;

	};
}

#endif