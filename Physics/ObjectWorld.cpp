#include "ObjectWorld.h"

void ObjectWorld::AddObject(Object* toAdd)
{
	physics::P6Particle* p = toAdd->getParticleAddress();
	Objects.push_back(toAdd);
	registry.add(p, &gravity);
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
