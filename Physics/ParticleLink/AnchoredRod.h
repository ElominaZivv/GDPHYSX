#ifndef ANCHOREDROD_H
#define ANCHOREDROD_H

#include "ParticleLink.h"
#include "AnchoredRodParticleContact.h"

namespace physics
{
	class AnchoredRod : public ParticleLink
	{
	public:
		//Length of the rod
		float length = 1;

		//Restitution
		float restitution = 0.9f;

		AnchoredRodParticleContact* GetContact() override;

	};
}

#endif