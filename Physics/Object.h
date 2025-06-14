#ifndef OBJECT_H
#define OBJECT_H

#include "../HeaderFiles/Model3D.h"
#include "../HeaderFiles/VAO.h"
#include "../HeaderFiles/Shader.h"
#include "../HeaderFiles/Camera.h"

#include <iostream>
#include <string>

using namespace std;
#include "Particle.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"

class Object
{
private:
	Model3D* model3D;
	physics::P6Particle particle;

public:
	//Constructor
	Object(shared_ptr<VAO> modelVAO);
	//Destructor
	~Object();

	void update(float dTime);
	void render(Shader shader, Camera camera);
	//Getters
	physics::P6Particle* getParticleAddress();

	//Setters
	void setColor(float r, float g, float b);
	void setSize(float _size);
	void setObjPos(float x, float y, float z);
	void setObjVel(float x, float y, float z);
	void setObjAcc(float x, float y, float z);
	void destroy();

	//Physics
	void addForce(physics::Vector force);
	void resetForce();

	//Getters
	physics::Vector getObjPos();
	bool isDestroyed();
};

class ObjectWorld
{
public:
	physics::ForceRegistry registry;

	std::list<Object*> Objects;

	void AddObject(Object* toAdd);
	void Update(float dTime);
	void Render(Shader shader, Camera camera);

	//Silly Functions
	void atCenter();

private:
	physics::GravityForceGenerator gravity = physics::GravityForceGenerator(physics::Vector(0, -9.8f, 0));
	void UpdateObjectList();
};

#endif