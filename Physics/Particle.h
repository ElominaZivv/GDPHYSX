#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <list>
using namespace std;

#include "Vector.h"

namespace physics {
	class P6Particle
	{
	protected:
		bool IsDestroyed = false;
		float damping = 0.99f;
		physics::Vector accumulatedForce = physics::Vector(0, 0, 0);

	public:
		//Mass of the particle
		float mass = 1.0f;
		float radius = 1.0f;
		float restitution = 1.0f;

		physics::Vector pos; //Position
		physics::Vector vel; //Velocity
		physics::Vector acc; //Acceleration

	protected:
		void updatePos(float dTime);
		void updateVel(float dTime);

	public:
		void update(float dTime);
		void destroy();
		bool isDestroyed();
		void addForce(physics::Vector force);
		void resetForce();
		void setDamping(float _damping);
	};

	class PhysicsWorld
	{
	public:
		std::list<P6Particle*> Particles;

		void AddParticle(P6Particle* toAdd);
		void Update(float dTime);

	private:
		void UpdateParticleList();
	};
}

#endif

/*
	May 23, 2025

	Vel = Delta P / Delta T		OR		(P2 - P1)/t
	P2 = P1 + VT; Find the next position

	Acc = (FinalVel - InitialVel)/t
	FinalVel = (Acc * t) + InitialVel

	P2 = P1 + (Vi*t + (A*t*t)/2)

*/