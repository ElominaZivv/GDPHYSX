#include "ContactResolver.h"

namespace physics
{
	void ContactResolver::ResolveContacts(std::vector<ParticleContact*> contacts, float time)
	{
		// Sort
		std::sort(	contacts.begin(),	// Beginning of vector
					contacts.end(),		// End of vector
					[]					// [] lambda function is an anonymous function called in-line
					(const ParticleContact* a, const ParticleContact* b)	//  Parameters of Lambda function
					{	
					return a->separatingSpeed < b->separatingSpeed;			// Lambda function definition
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