#include "ObjectWorld.h"

void ObjectWorld::AddObject(Object* toAdd)
{
	physics::P6Particle* p = toAdd->getParticleAddress();
	Objects.push_back(toAdd);
	//registry.add(p, &gravity); // Remove Gravity for now
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

void ObjectWorld::AddContact(physics::P6Particle* p1, physics::P6Particle* p2, float restitution, physics::Vector contactNormal)
{
	//Create Particle Contact
	physics::ParticleContact *toAdd = new physics::ParticleContact();

	toAdd->particles[0] = p1;
	toAdd->particles[1] = p2;
	toAdd->restitution = restitution;
	toAdd->contactNormal = contactNormal;

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
