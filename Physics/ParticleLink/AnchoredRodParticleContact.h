#ifndef ANCHOREDRODPARTICLECONTACT_H
#define ANCHOREDRODPARTICLECONTACT_H

#include "../ParticleContact.h"

namespace physics
{
	class AnchoredRodParticleContact : public ParticleContact
	{
	public:
		void Update() override;
		void Resolve(float time) override;
	protected:
		void ResolveVelocity(float time) override;
		void ResolveInterpenetration(float time) override;
	};
}

#endif