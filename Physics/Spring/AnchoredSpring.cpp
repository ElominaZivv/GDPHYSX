#include "AnchoredSpring.h"

namespace physics
{
	void AnchoredSpring::updateForce(P6Particle* particle, float time)
	{
		Vector pos = particle->pos;
		Vector force = pos - anchorPoint;
		float mag = force.mag();

		//			f		=	-k				*	absolute val of (lnCurr - lnRest)
		float springForce	=	-springConstant *	abs(mag - restLength);

		force.normalize();
		force = force * springForce;

		particle->addForce(force);
	}
}