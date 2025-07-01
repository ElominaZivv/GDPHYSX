#include "ParticleContact.h"

void physics::ParticleContact::Update()
{
	// Compute and Update the values of separating speed and depth
	GetSeparatingSpeed();
	//GetDepth();
}

void physics::ParticleContact::Resolve(float time)
{
	//Conditional (Only change velocity if the particles collide
	if (depth>0.0f) ResolveVelocity(time);
	//Order does not really matter [[am i being gaslit?]]
	ResolveInterpenetration(time);
}

float physics::ParticleContact::GetSeparatingSpeed()
{
	//Av
	Vector velocity = particles[0]->vel;
	//					Av			-		Bv
	if (particles[1]) velocity -= particles[1]->vel;

	// Update fSeparatingSpeed
	//		(Av-Bv) dot N
	fSeparatingSpeed = velocity.dot(contactNormal);

	return fSeparatingSpeed;
}

float physics::ParticleContact::GetDepth()
{
	Vector particlePosDifference = particles[0]->pos;
	if (particles[1]) particlePosDifference -= particles[1]->pos;

	//Update depth
	// Depth is the magnitude of the distance between centers minus the radii of the 2 particles.
	//60.0f is hardcoded since the model size is set to 30 and 30
	depth =  60 - particlePosDifference.mag();
	
	return depth;
}

void physics::ParticleContact::ResolveVelocity(float time)
{
	//Compute for separating speed (sS)
	float separatingSpeed = GetSeparatingSpeed();

	//If the objects are already moving away from each other, skip
	if (separatingSpeed > 0) return;

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

void physics::ParticleContact::ResolveInterpenetration(float time)
{
	//Compute Depth
	//GetDepth();

	// If they are not overlapping, skip
	if (depth <= 0) return;

	//Get total mass of the collision
	float totalMass = (float)1 / particles[0]->mass;
	if (particles[1]) totalMass += (float)1 / particles[1]->mass;

	//Invalid collision if total mass is <= 0
	if (totalMass <= 0) return;

	//How many units to move per total mass
	float totalMoveByMass = depth / totalMass;
	 
	//Get the vector of the total movement involved
	Vector moveByMass = contactNormal * totalMoveByMass;

	//Get in the change in position of the first particle
	Vector P_a = moveByMass * ((float)1 / particles[0]->mass);
	//Translate a
	particles[0]->pos += P_a;

	if (particles[1])
	{
		//Get in the change in position of the second particle
		Vector P_b = moveByMass * (-(float)1 / particles[1]->mass);
		//Translate b
		particles[1]->pos += P_b;
	}

	//Assume the interpenetration was resolved and set depth to 0
	depth = 0;
}
