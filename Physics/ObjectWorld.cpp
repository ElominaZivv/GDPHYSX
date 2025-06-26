#include "ObjectWorld.h"

void ObjectWorld::AddObject(Object* toAdd)
{
	physics::P6Particle* p = toAdd->getParticleAddress();

	random_device random;
	uniform_real_distribution<float> force(-10000.f, 10000.f);
	float minimumAmplitude = 5000.f;
	physics::Vector forceVector = physics::Vector(force(random), std::abs(force(random)) + minimumAmplitude, force(random));
	forceVector.normalize();
	(p)->addForce(forceVector * std::abs(force(random)));

	Objects.push_back(toAdd);
	registry.add(p, &gravity);
}

void ObjectWorld::Render(Shader shader, Camera camera)
{
	//Render all Objects Sequentially

	int rendered = 0;
	renderedObject = Objects.size();
	for (std::list<Object*>::iterator obj = Objects.begin();
		obj != Objects.end() && rendered < renderedObject;
		++obj, ++rendered)
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
}

void ObjectWorld::UpdateObjectList()
{
	//remove obj from the registry
	for (Object* obj : Objects) if (obj->isDestroyed()) registry.remove(obj->getParticleAddress(), &gravity);

	Objects.remove_if(
		[](Object* obj) {
			return obj->isDestroyed();
		}
	);
}
