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
	//	Phase2
	float radius;

public:
	//Constructor
	Object();
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
	void setRadius(float _radius);
	void setMass(float _mass);
	void setObjPos(float x, float y, float z);
	void setObjVel(float x, float y, float z);
	void setObjAcc(float x, float y, float z);
	void destroy();
	void setRestitution(float _restitution);

	//Physics
	void addForce(physics::Vector force);
	void resetForce();

	//Getters
	physics::Vector getObjPos();
	float GetRadius();
	float GetRestitution();
	bool isDestroyed();
};

#endif