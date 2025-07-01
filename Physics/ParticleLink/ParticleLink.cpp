#include "ParticleLink.h"

namespace physics
{
	float ParticleLink::CurrentLength()
	{
		Vector ret = particles[0]->pos - particles[1]->pos;
		return ret.mag();
	}
}