#include "Object.h"

//Constructor
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

	//	Change color of model based on speed
	//	Gompertz Growth Equation = a * e^(-Be^(-k(magnitude)))
	//	a = upper limit
	//	k = some constant >0
	//	B = distance of slope from y-axis
	float a = 1.0f;	//Shader RGB values are from 0.0 -> 1.0
	float k = 0.075f;
	float b = 15.f;	// Steepness of curve / How long it takes to get from the lower limit to the upper limit, a.

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

// +------------------------ OBJECT WORLD ------------------------+

void ObjectWorld::AddObject(Object* toAdd)
{
	physics::P6Particle* p = toAdd->getParticleAddress();
	Objects.push_back(toAdd);
	//registry.add(p, &gravity); // Remove Gravity for now
}

void ObjectWorld::Render(Shader shader, Camera camera)
{
	//Render all Objects
	for (std::list<Object*>::iterator obj = Objects.begin();
		obj != Objects.end();
		obj++
		)
	{
		(*obj)->render(shader, camera);
	}
}

void ObjectWorld::Update(float dTime)
{	
	//Silly Functions
	//atCenter();

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
	Objects.remove_if(
		[](Object* obj) {
			return obj->isDestroyed();
		}
	);

}

//Silly Functions
void ObjectWorld::atCenter()
{
	// The distance of the side of a collision box centered at (0,0,0)
	float side=0.5f;

	// If a particle is at the center, destory particle
	for (std::list<Object*>::iterator obj = Objects.begin();
		obj != Objects.end();
		obj++
		)
	{
		//Define collision box
		/*
			
				"+" is the Center, (0,0,0)

										Upper Right Corner
				------------------------*
				|						| 
				|						| 
				|						| 
				|						|
				| 			+  <-side->	| 
				|						| 
				|						| 
				|						|
				|						|
				*------------------------
				Lower Left Corner
		*/

		//								Lower Left Corner											Upper Right Corner
		if ((*obj)->getObjPos() >= physics::Vector(-side,-side,-side) && (*obj)->getObjPos() <= physics::Vector(side,side,side))
		{
			(*obj)->destroy();
		}
	}
}
