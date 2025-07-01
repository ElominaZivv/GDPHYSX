#ifndef CONTACTRESOLVER_H
#define CONTACTRESOLVER_H

#include "ParticleContact.h"
#include <algorithm>
#include <vector>

namespace physics
{
	class ContactResolver
	{
	protected:
		unsigned current_iterations = 0;
	public:
		//Max number of Resolve Calls
		unsigned max_iterations;
		ContactResolver(unsigned _maxIterations) : max_iterations(_maxIterations) {};
		//Resolve all assigned contacts
		void ResolveContacts(std::vector<ParticleContact*> contacts, float time);
		void resetCurrentIteration();
	};
}

#endif