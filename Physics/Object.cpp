#include "Object.h"

//Constructor
Object::Object(string _name, shared_ptr<VAO> modelVAO)
{
	name = _name;
	model3D = new Model3D(modelVAO);
};

//Destructor
Object::~Object()
{
	delete model3D;
}

void Object::update(float dTime)
{
	particle.update(dTime);
	//Set the position of the model to the position of its particle component
	model3D->modelPos = vec3(particle.pos);

	race_time += dTime;

	//	Change color of model based on speed
	//	Gompertz Growth Equation = a * e^(-Be^(-k(magnitude)))
	//	a = upper limit
	//	k = some constant > 0 ; steepness of curve 
	//	B = Distance of the base of the slope from y-axis
	float a = 1.0f;		// Shader RGB values are from 0.0 -> 1.0
	float k = 0.015f;	// Steepness of curve ; higher the val, the more steep the curve
	float b = 15.f;		// Distance of the base of the slope from y-axis

	float x = k * particle.vel.mag(); 
	float be = b * powf(2.718, -x);
	float n = a * powf(2.718, -be);

	float erm = 0.5f;
	float um = 0.2f;

	model3D->color = glm::normalize(glm::vec3(n, um+(erm-(n *erm)), 1-n));
}

void Object::render(Shader shader, Camera camera)
{
	model3D->draw(shader, camera);
}

physics::P6Particle* Object::getParticleAddress()
{
	return &particle;
}

bool Object::getPause()
{
	return isPaused;
}

string Object::getName()
{
	return name;
}

void Object::setColor(float r, float g, float b)
{
	model3D->color = vec3(r, g, b);
}

void Object::setSize(float _size)
{
	model3D->setSize(_size);
}

void Object::setObjPos(float x, float y, float z)
{
	particle.pos = physics::Vector(x, y, z);
}

void Object::setObjVel(float x, float y, float z)
{

	particle.vel = physics::Vector(x, y, z);
}

void Object::setObjAcc(float x, float y, float z)
{
	particle.acc = physics::Vector(x, y, z);
}

void Object::setPause(bool val)
{
	isPaused = val;
}

void Object::destroy()
{
	particle.destroy();
}

void Object::addForce(physics::Vector force)
{
	particle.addForce(force);
}

void Object::resetForce()
{
	particle.resetForce();
}

physics::Vector Object::getObjPos()
{
	return particle.pos;
}

float Object::getRaceTime()
{
	return race_time;
}

bool Object::isDestroyed() { return particle.isDestroyed(); }
