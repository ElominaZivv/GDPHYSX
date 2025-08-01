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
}

void Object::render(Shader shader, Camera camera)
{
	model3D->draw(shader, camera);
}

physics::P6Particle* Object::getParticleAddress()
{
	return &particle;
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

void Object::setRestitution(float _restitution)
{
	particle.restitution = _restitution;
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

