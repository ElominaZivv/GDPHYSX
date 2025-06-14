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
protected:
	Model3D* model3D;
	physics::P6Particle particle;

	string name;
	bool isPaused = false;
	float race_time = 0.0;

public:
	//Constructor
	Object(string _name, shared_ptr<VAO> modelVAO);
	//Destructor
	~Object();

	virtual void update(float dTime);
	void render(Shader shader, Camera camera);
	//Getters
	physics::P6Particle* getParticleAddress();
	bool getPause();
	string getName();

	//Setters
	void setColor(float r, float g, float b);
	void setSize(float _size);
	void setObjPos(float x, float y, float z);
	void setObjVel(float x, float y, float z);
	void setObjAcc(float x, float y, float z);
	void setPause(bool val);
	void destroy();

	//Physics
	void addForce(physics::Vector force);
	void resetForce();

	//Getters
	physics::Vector getObjPos();
	float getRaceTime();
	bool isDestroyed();
};

#endif