#include "DragForceGenerator.h"

namespace physics
{

	void DragForceGenerator::updateForce(physics::P6Particle* particle, float dTime)
	{
		physics::Vector force = physics::Vector(0, 0, 0); // What am I for?
		physics::Vector currV = particle->vel;

		float mag = currV.mag();
		if (mag <= 0.00f) return;

		//Formula for drag v v v v v v v v v 
		float dragF = (k1 * mag) + (k2 * mag);
		physics::Vector dir = currV.dir();
			
		particle->addForce(dir * -dragF);
	}

}	