#ifndef DRAGFORCEGENERATOR_H
#define DRAGFORCEGENERATOR_H

#include "ForceGenerator.h"
#include "Particle.h"

namespace physics
{

	class DragForceGenerator : public ForceGenerator
	{
	private:
		// Coefficient of Frictions
		float k1 = 0.74f;
		float k2 = 0.57f;
	public:
		DragForceGenerator() {}
		DragForceGenerator(float _k1, float _k2) : k1(_k1), k2(_k2) {};	
		void updateForce(physics::P6Particle* p, float time) override;
	};

}

#endif