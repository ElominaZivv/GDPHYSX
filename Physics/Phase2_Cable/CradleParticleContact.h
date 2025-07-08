#ifndef CRADLEPARTICLECONTACT_H
#define CRADLEPARTICLECONTACT_H

#include "../ParticleContact.h"

namespace physics
{
	class CradleParticleContact : public ParticleContact
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