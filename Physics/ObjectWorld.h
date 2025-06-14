#ifndef OBJECTWORLD_H
#define OBJECTWORLD_H

#include "Object.h"

namespace physics
{
	class ObjectWorld
	{
	public:
		float finishLine = 1000.0f;
		int rank = 0;

		physics::ForceRegistry registry;

		std::list<Object*> Objects;

		void AddObject(Object* toAdd);
		void Update(float dTime);
		void Render(Shader shader, Camera camera);


		//Silly Functions
		void atCenter();

	private:
		void UpdateObjectList();
		void FinishLine();
	};
}


#endif