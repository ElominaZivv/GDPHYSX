#include "Particle.h"

namespace physics{
	void P6Particle::updatePos(float dTime)
	{
		// P2	=	P1	+	V	dTime
		//pos = pos + (vel *= dTime);
		// P2 = P1 + (ViT) + ((At^2)/2)
		pos = pos + (vel * dTime) + ((acc * (dTime * dTime)) * (1.0 / 2.0));

	}
	void P6Particle::updateVel(float dTime)
	{
		//	F			=	a					*		m
			this->acc	+=	accumulatedForce	*	(1.f / mass);

			vel = vel + (acc * dTime);

		//				vF			d^t
			this->vel = this->vel * powf(damping, dTime);
	}
	void P6Particle::update(float dTime) {//this is to be called by the engine
		//Always call updatePos BEFORE updateVel
		this->updatePos(dTime);
		this->updateVel(dTime);
		resetForce();
	}
	void P6Particle::destroy()
	{
		this->IsDestroyed = true;
	}
	bool P6Particle::isDestroyed()
	{
		return this->IsDestroyed;
	}

	void P6Particle::addForce(physics::Vector force)
	{
		this->accumulatedForce += force;
	}

	void P6Particle::resetForce()
	{
		this->accumulatedForce = physics::Vector(0, 0, 0);
		this->acc = physics::Vector(0, 0, 0);
	}

	void P6Particle::decreaseDamping() {
		damping -= 0.001;
	}
	float P6Particle::getDamping() {
		return damping;
	}

	void PhysicsWorld::AddParticle(P6Particle* toAdd)
	{
		Particles.push_back(toAdd);
	}

	void PhysicsWorld::Update(float dTime)
	{
		UpdateParticleList();

		//Update all particles
		for (std::list<P6Particle*>::iterator p = Particles.begin();
			p != Particles.end();
			p++
			)
		{
			(*p)->update(dTime);
		}
	}

	void PhysicsWorld::UpdateParticleList()
	{
		/*
			
		*/
		Particles.remove_if(
			[](P6Particle* p) {
				return p->isDestroyed();
			}
		);

	}

}

/*
	
	Apply Force
	F=mA

*/