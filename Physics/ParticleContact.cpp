#include "ParticleContact.h"

bool physics::ParticleContact::operator<(ParticleContact& p)
{
	return separatingSpeed < p.separatingSpeed;
}

void physics::ParticleContact::Resolve(float time)
{
	ResolveVelocity(time);
}

float physics::ParticleContact::GetSeparatingSpeed()
{
	//Av
	Vector velocity = particles[0]->vel;
	//					Av			-		Bv
	if (particles[1]) velocity -= particles[1]->vel;

	separatingSpeed = velocity.dot(contactNormal);
	//		(Av-Bv) dot N
	return separatingSpeed;
}

void physics::ParticleContact::ResolveVelocity(float time)
{
	//sS
	float separatingSpeed = GetSeparatingSpeed();

	//If the objects are already moving away from each other, skip
	if (separatingSpeed > 0)
	{
		return;
	}

	//Get the separating speed after the collision
	// SAfter	=	-c		*	sS
	float newSS = -restitution * separatingSpeed;
	// Magnitude of our delta velocity
	float deltaSpeed = newSS - separatingSpeed;

	//Get Total Inverse Mass of Particles
	// Apply impulse based on this later
	float totalMass = (float)1 / particles[0]->mass;
	if (particles[1]) totalMass += (float)1 / particles[1]->mass;

	//Invalid collision if total mass is 0 or less
	if (totalMass <= 0)return;

	//Magnitude of impulse needed to apply in this collision
	//			i		=	mass	*	deltaSpeed
	float impulase_mag = deltaSpeed / totalMass;
	// Get the impulse vector using the	contact normal
	Vector impulse = contactNormal * impulase_mag;

	//Apply changes in velocity of A by dividng the impulse vector with the mass of A
	//Changes in V for Particle A
	//		 V =	I	 /				M
	Vector V_a = impulse * ((float)1 / particles[0]->mass);
	//			Vf	  =		Vi			  +	I/m
	particles[0]->vel = particles[0]->vel + V_a;

	//For VectorB but in the opposite direction
	Vector V_b = impulse * ((float)-1 / particles[1]->mass);
	//			Vf	  =		Vi			  +	I/m
	particles[1]->vel = particles[1]->vel + V_b;


}
