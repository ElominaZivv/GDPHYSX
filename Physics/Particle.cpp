#include "Particle.h"

namespace physics {
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
		this->acc += accumulatedForce * (1.f / mass);

		vel = vel + (acc * dTime);

		//				vF			d^t
		this->vel = this->vel * powf(damping, dTime);
	}

	void P6Particle::updateRotation(float dTime)
	{
		// Update Rotation
		Vector angularV = angularVel * dTime;
		//Get Mag
		float angleMag = 0.0f;
		angleMag = angularV.mag();
		//Get Dir
		Vector magDir = angularV.dir();

		if (angleMag != 0)
		{
			glm::quat rotBy = glm::rotate(
				glm::mat4(1.f),
				angleMag,
				(glm::vec3)magDir
			);


			this->rotation =	
				glm::toMat4
				(
					//convert mat4 rotation to quaternion
					glm::toQuat(this->rotation) * rotBy //Multiply the 2 quaternions
				);
		}
	}

	float P6Particle::MomentOfIntertia()
	{
		return ((float)2 / 5) * mass * radius *radius;
	}

	void P6Particle::computeAngularVelocity(float dTime)
	{
		float mI = MomentOfIntertia();
		angularVel += accummulatedTorque * ((float)1 / mI) * dTime;
		angularVel = angularVel * powf(angularDampening, dTime);
	}

	void P6Particle::update(float dTime) {//this is to be called by the engine
		//Always call updatePos BEFORE updateVel
		this->updatePos(dTime);
		this->updateVel(dTime);
		this->updateRotation(dTime);
		this->computeAngularVelocity(dTime);
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

	void P6Particle::AddForceAtPoint(physics::Vector force, physics::Vector refPoint)
	{
		addForce(force);
		this->accummulatedTorque = refPoint.cross(force);
	}

	void P6Particle::resetForce()
	{
		this->accumulatedForce = physics::Vector(0, 0, 0);
		this->accummulatedTorque = physics::Vector(0, 0, 0);
		this->acc = physics::Vector(0, 0, 0);
	}
}

/*
	
	Apply Force
	F=mA

*/