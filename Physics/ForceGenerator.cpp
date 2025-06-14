#include "ForceGenerator.h"

using namespace physics;

void ForceGenerator::updateForce(physics::P6Particle* p, float time)
{
	p->addForce(Vector(0, 0, 0));
}

