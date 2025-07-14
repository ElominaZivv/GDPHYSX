#ifndef OBJECTWORLD_H
#define OBJECTWORLD_H

#include "../HeaderFiles/Model3D.h"
#include "../HeaderFiles/VAO.h"
#include "../HeaderFiles/Shader.h"
#include "../HeaderFiles/Camera.h"

#include <iostream>
#include <string>

using namespace std;
#include "Particle.h"
#include "Object.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"
#include "ParticleContact.h"
#include "ContactResolver.h"
#include "ParticleLink/Rod.h"

class ObjectWorld
{
protected:
	void GenerateContacts();
	physics::ContactResolver contactResolver = physics::ContactResolver(20);
	void GetOverlaps();

public:
	physics::ForceRegistry registry;
		
	std::list<Object*> Objects;
	std::list<physics::ParticleLink*>Links;
	void AddObject(Object* toAdd);

	std::vector<physics::ParticleContact*> Contacts;
	void AddContact(physics::P6Particle* p1, physics::P6Particle* p2, float restitution, physics::Vector contactNormal, float depth);

	void Update(float dTime);
	void Render(Shader shader, Camera camera);

	//Silly Functions
	void atCenter();

private:
	physics::GravityForceGenerator gravity = physics::GravityForceGenerator(physics::Vector(0, -9.8f, 0));
	void UpdateObjectList();
};

#endif