#include "Object.h"

//Constructor
Object::Object(shared_ptr<VAO> modelVAO)
{
	model3D = new Model3D(modelVAO);

	//Phase1
	RandomizeColor();
	RandomizeRadius();
	RandomizeLifeSpan();
};

//Destructor
Object::~Object()
{
	delete model3D;
}

void Object::update(float dTime)
{
	CheckLifeSpan(); //Checks if the object has surpassed its life time
	particle.update(dTime);

	//Set the position of the model to the position of its particle component
	model3D->modelPos = vec3(particle.pos);

	//Phase1
	//Decrement DeltaTime from LifeSpan
	lifeSpan -= dTime;
}

void Object::RandomizeColor()
{
	model3D->color = glm::vec3(rngFloat(0.0, 1.0), rngFloat(0.0, 1.0), rngFloat(0.0, 1.0));
}

void Object::RandomizeRadius()
{
	model3D->setSize(rngFloat(2.0, 10.0));
}

void Object::RandomizeLifeSpan()
{
	lifeSpan = rngFloat(1.0, 10.0);
}

void Object::CheckLifeSpan()
{
	if (lifeSpan <= 0.0f)
	{
		destroy();
	}
}

void Object::MySillyFunctionThatChangesTheColorOfTheModelBasedOnItsSpeed()
{
	//	Change color of model based on speed
	//	Gompertz Growth Equation = a * e^(-Be^(-k(magnitude)))
	//	a = upper limit
	//	k = some constant > 0 ; steepness of slope
	//	B = distance of slope from y-axis
	float a = 1.0f;
	float k = 0.9f;
	float b = 2.f;

	float x = k * particle.vel.mag();
	float be = b * powf(2.718, -x);
	float n = a * powf(2.718, -be);

	float erm = 0.5f;
	float umm = 0.2f;

	model3D->color = glm::normalize(glm::vec3(n, umm + (erm - (n * erm)), 1 - n));
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

void Object::setSize(float _size)
{
	model3D->setSize(_size);
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

bool Object::isDestroyed() { return particle.isDestroyed(); }

