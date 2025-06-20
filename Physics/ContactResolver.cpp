#include "ContactResolver.h"

namespace physics
{
	void ContactResolver::ResolveContacts(std::vector<ParticleContact*> contacts, float time)
	{
		std::sort(contacts.begin(), contacts.end(), [](const ParticleContact*& a, const ParticleContact*& b) {
			return a < b;
		});

		if (current_iterations <= max_iterations)
		{
			for (unsigned i = 0; i < contacts.size(); i++)
			{
				contacts[i]->Resolve(time);
			}
			current_iterations++;
		}
	}
}	