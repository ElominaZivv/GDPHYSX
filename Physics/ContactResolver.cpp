#include "ContactResolver.h"

namespace physics
{
	void ContactResolver::ResolveContacts(std::vector<ParticleContact*> contacts, float time)
	{
		int contactSize = contacts.size();
		int check = 0;
		while (check != contactSize && current_iterations < max_iterations)
		{
			//Compute the separating speed of the contacts
			for (unsigned i = 0; i < contactSize; i++) contacts[i]->Update();

			// [[1]] Get contact with the least separatingSpeed or a contact with depth 0
			unsigned index = 0;
			float depth = 0.0f;
			float lowestSpeed = 0.0f;
			float sS = 0.0f;
			for (unsigned i = 0; i < contactSize; i++)
			{
				sS = contacts[i]->fSeparatingSpeed;
				if (sS < lowestSpeed)
				{
					lowestSpeed = sS;
					index = i;
				}

				if (contacts[i]->depth < 0)
				{
					index = i;
					i = contactSize; // Break for loop
				}
			}

			/* SCRAPPED
			std::sort(contacts.begin(),	// Beginning of vector
				contacts.end(),		// End of vector
				[]					// [] lambda function is an anonymous function called in-line
			(const ParticleContact* a, const ParticleContact* b)	//  Parameters of Lambda function
				{
					return a->fSeparatingSpeed < b->fSeparatingSpeed || a->depth < 0.0f;			// Lambda function definition
				});
			SCRAPPED	*/

			// [[2]] Resolve the contact
			contacts[index]->Resolve(time);

			// [[3]] Increment resolve count
			current_iterations++;

			// [[4]] if sS of all contacts are >= 0 AND Depth <=0, end
			check = 0;
			for (unsigned i = 0; i < contactSize; i++)
			{
				if (contacts[i]->fSeparatingSpeed >= 0 && contacts[i]->depth <= 0)
				{
					check++;
				}
			}
		}
	}
	void ContactResolver::resetCurrentIteration()
	{
		current_iterations = 0;
	}
}	