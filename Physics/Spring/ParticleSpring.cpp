#include "ParticleSpring.h"

void physics::ParticleSpring::updateForce(P6Particle* particle, float time)
{
	Vector pos = particle->pos;
	Vector force = pos - otherParticle->pos;
	float mag = force.mag();

	//			f		=	-k				*	absolute val of (lnCurr - lnRest)
	float springForce = -springConstant * abs(mag - restLength);

	force.normalize();
	force = force * springForce;

	particle->addForce(force);
}
