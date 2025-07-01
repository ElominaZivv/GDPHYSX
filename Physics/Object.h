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

	//My Silly Functions
	void MySillyFunctionThatChangesTheColorOfTheParticleBasedOnItsSpeed();

	//Setters
	void setColor(float r, float g, float b);
	void setSize(float _size);
	void setMass(float _mass);
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

#endif