#include "ObjectWorld.h"

void ObjectWorld::GenerateContacts()
{
	//Erase all contacts
	Contacts.clear();

	//Collision Detection and add contact if collision
	GetOverlaps();

	for (std::list<physics::ParticleLink*>::iterator i = Links.begin();
		i != Links.end();
		i++)
	{
		physics::ParticleContact* contact = (*i)->GetContact();
		if (contact != nullptr)
		{
			Contacts.push_back(contact);
		}
	}

	// Gods forgive me for what I am about to do
	// Should add some collision detecttion so this does not get called every update
	physics::Vector dir;
	for (std::list<Object*>::iterator obj = Objects.begin(); obj != Objects.end(); obj++)
	{
		auto n_obj = obj;
		if (obj != prev(Objects.end(),1))
		{
			advance(n_obj, 1);
			dir = (*obj)->getObjPos() - (*n_obj)->getObjPos();
			dir.normalize();
			AddContact
			(
				(*obj)->getParticleAddress(),
				(*n_obj)->getParticleAddress(),
				(*obj)->getSize(),
				(*n_obj)->getSize(),
				0.9,
				dir
			);
		}
	}
}

void ObjectWorld::GetOverlaps()
{
	//Iterate through the list up to the 2nd to the last element
	// ex {A, B, C, D} -> Iterate from A->C
	for (int i = 0; i < Objects.size() - 1; i++)
	{
		std::list<Object*>::iterator a = std::next(Objects.begin(), i);

		//Start with the next element from above
		// ex {A, B, C, D} -> Iterate from B->D
		for (int h = i + 1; h < Objects.size(); h++)
		{
			std::list<Object*>::iterator b = std::next(Objects.begin(), h);

			//Get the vector from A to B
			physics::Vector mag2Vector = (*a)->getObjPos() - (*b)->getObjPos();
			
			//Get the square magnitude	
			float mag2 = mag2Vector.squareMag();

			//Get the sum of the radius of A and B
			float rad = (*a)->GetRadius() + (*b)->GetRadius();
			//Square the rad
			float rad2 = rad * rad;

			//If Sq. Magnitude is == to Sq. Sum- They are touching
			//If Sq. Magnitude is < to Sq. Sum- They are overlapping
			if (mag2 <= rad2)
			{
				//Get the direction of A->B
				physics::Vector dir = mag2Vector.dir();
				float r = rad2 - mag2;
				float depth = sqrt(r);

				//Use the lower restitution of the 2 particles
				float restitution = fmin((*a)->GetRestitution(), (*b)->GetRestitution());

				AddContact((*a)->getParticleAddress(), (*b)->getParticleAddress(), restitution, dir, depth);
			}
		}

	}
}

void ObjectWorld::AddObject(Object* toAdd)
{
	physics::P6Particle* p = toAdd->getParticleAddress();
	Objects.push_back(toAdd);
	//registry.add(p, &gravity); // Remove Gravity for now
}

void ObjectWorld::AddObject(Object* toAdd, bool affectedByGravity)
{
	physics::P6Particle* p = toAdd->getParticleAddress();
	Objects.push_back(toAdd);
	if(affectedByGravity)registry.add(p, &gravity); // Remove Gravity for now
}

void ObjectWorld::Render(Shader shader, Camera camera)
{
	//Render all Objects
	for (std::list<Object*>::iterator obj = Objects.begin();
		obj != Objects.end();
		obj++
		)
	{
		(*obj)->render(shader, camera);
	}
}

void ObjectWorld::AddContact(physics::P6Particle* p1, physics::P6Particle* p2, float restitution, physics::Vector contactNormal, float depth)
{
	//Create Particle Contact
	physics::ParticleContact *toAdd = new physics::ParticleContact();

	toAdd->particles[0] = p1;
	toAdd->particles[1] = p2;
	toAdd->particleRadii[0] = particleRadius1;
	toAdd->particleRadii[1] = particleRadius2;
	toAdd->restitution = restitution;
	toAdd->contactNormal = contactNormal;
	toAdd->depth = depth;

	Contacts.push_back(toAdd);
}

void ObjectWorld::Update(float dTime)
{
	//Silly Functions
	//atCenter();

	UpdateObjectList();

	registry.updateForces(dTime);

	//Update all objects
	for (std::list<Object*>::iterator obj = Objects.begin();
		obj != Objects.end();
		obj++
		)
	{
		(*obj)->update(dTime);
	}

	GenerateContacts();

	if (Contacts.size() > 0)
	{
		contactResolver.ResolveContacts(Contacts, dTime);
	}
}

void ObjectWorld::UpdateObjectList()
{
	Objects.remove_if(
		[](Object* obj) {
			return obj->isDestroyed();
		}
	);

}

//Silly Functions
void ObjectWorld::atCenter()
{
	// The distance of the side of a collision box centered at (0,0,0)
	float side = 0.5f;

	// If a particle is at the center, destory particle
	for (std::list<Object*>::iterator obj = Objects.begin();
		obj != Objects.end();
		obj++
		)
	{
		//Define collision box
		/*

				"+" is the Center, (0,0,0)

										Upper Right Corner
				------------------------*
				|						|
				|						|
				|						|
				|						|
				| 			+  <-side->	|
				|						|
				|						|
				|						|
				|						|
				*------------------------
				Lower Left Corner
		*/

		//								Lower Left Corner											Upper Right Corner
		if ((*obj)->getObjPos() >= physics::Vector(-side, -side, -side) && (*obj)->getObjPos() <= physics::Vector(side, side, side))
		{
			(*obj)->destroy();
		}
	}
}
