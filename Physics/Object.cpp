#include "Object.h"

//Constructor
Object::Object()
{};
Object::Object(shared_ptr<VAO> modelVAO)
{
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

	MySillyFunctionThatChangesTheColorOfTheParticleBasedOnItsSpeed();
}

void Object::render(Shader shader, Camera camera)
{
	model3D->draw(shader, camera);
}

physics::P6Particle* Object::getParticleAddress()
{
	return &particle;
}

void Object::MySillyFunctionThatChangesTheColorOfTheParticleBasedOnItsSpeed()
{
	//	Change color of model based on speed
	//	Gompertz Growth Equation = a * e^(-Be^(-k(magnitude)))
	//	a = upper limit
	//	k = some constant > 0 ; steepness of slope
	//	B = distance of slope from y-axis
	float a = 1.0f;
	float k = 0.015f;
	float b = 1.0f;

	float x = k * particle.vel.mag();
	float be = b * powf(2.718, -x);
	float n = a * powf(2.718, -be);

	float erm = 0.5f;
	float umm = 0.2f;

	model3D->color = glm::normalize(glm::vec3(n, umm + (erm - (n * erm)), 1 - n));
}

void Object::setColor(float r, float g, float b)
{
	model3D->color = vec3(r, g, b);
}

void Object::setRadius(float _radius)
{
	particle.radius = _radius;
	model3D->setSize(particle.radius);
}

void Object::setMass(float _mass)
{
	particle.mass = _mass;
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

float Object::GetRadius()
{
	return particle.radius;
}

float Object::GetRestitution()
{
	return particle.restitution;
}

bool Object::isDestroyed() { return particle.isDestroyed(); }

