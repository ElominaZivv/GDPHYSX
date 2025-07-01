#ifndef PARTICLESPRING_H
#define PARTICLESPRING_H

#include "../ForceGenerator.h"

namespace physics
{
	class ParticleSpring : public ForceGenerator
	{
	public:
		ParticleSpring(P6Particle* particle, float _springConstant, float _restLength) :
			otherParticle(particle), springConstant(_springConstant), restLength(_restLength) {};

		void updateForce(P6Particle* particle, float time) override;
	private:
		P6Particle* otherParticle;

		float springConstant;
		float restLength;
	};
}

#endif