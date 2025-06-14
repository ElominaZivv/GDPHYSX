#include "ObjectWorld.h"

namespace physics
{

	void ObjectWorld::AddObject(Object* toAdd)
	{
		physics::P6Particle* p = toAdd->getParticleAddress();
		Objects.push_back(toAdd);
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
		FinishLine();

		UpdateObjectList();

		registry.updateForces(dTime);

		//Update all objects
		for (std::list<Object*>::iterator obj = Objects.begin();
			obj != Objects.end();
			obj++
			)
		{
			if (!(*obj)->getPause())(*obj)->update(dTime);
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

	void ObjectWorld::FinishLine()
	{
		for (std::list<Object*>::iterator obj = Objects.begin();
			obj != Objects.end();
			obj++
			)
		{
			if ((*obj)->getObjPos().x >= finishLine)
			{
				if (!(*obj)->getPause()) {
					rank++;
					std::cout << "RANK [" << rank << "] : " << (*obj)->getName() << "	TIME: " << (*obj)->getRaceTime() << "sec";
					if (rank == 4)
					{
						std::cout << " (LAST PLACE)";
					}
					std::cout << std::endl;
				}
				(*obj)->setPause(true);
			}
		}
	}

	//Silly Functions
	void ObjectWorld::atCenter()
	{
		// The distance of the side of a collision box centered at (0,0,0)
		float side = 0.5f;

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
			if ((*obj)->getObjPos() >= physics::Vector(-side, -side, -side) && (*obj)->getObjPos() <= physics::Vector(side, side, side))
			{
				(*obj)->destroy();
			}
		}
	}

}
