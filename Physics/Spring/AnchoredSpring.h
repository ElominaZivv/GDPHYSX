#ifndef SPRING_H
#define SPRING_H

#include "../ForceGenerator.h"
#include "../Vector.h"

namespace physics
{
	class AnchoredSpring : public ForceGenerator
	{
	public: 
		AnchoredSpring(Vector pos, float _springConstant, float _restLength) :
			anchorPoint(pos), springConstant(_springConstant), restLength(_restLength) {};

		void updateForce(P6Particle* particle, float time) override;

	private:
		//Attach Particle
		Vector anchorPoint;

		//K, how stiff the spring is
		float springConstant;

		//Length of the spring
		float restLength;
	};
}

#endif