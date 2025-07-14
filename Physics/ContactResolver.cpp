#include "ContactResolver.h"

namespace physics
{
	void ContactResolver::ResolveContacts(std::vector<ParticleContact*> contacts, float time)
	{
		current_iterations = 0;
		int contactSize = contacts.size();
		while (current_iterations < max_iterations)
		{
			//Compute the separating speed of the contacts
			for (unsigned i = 0; i < contactSize; i++) contacts[i]->Update();

			// [[1]] Get contact with the least separatingSpeed or a contact with depth 0
			/*
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
			*/

			// Sort first by separating speed
			std::sort
			(
				contacts.begin(),	// Beginning of vector
				contacts.end(),		// End of vector
				[]					// [] lambda function is an anonymous function called in-line
				(const ParticleContact* a, const ParticleContact* b)	//  Parameters of Lambda function
				{
					//Sort the particle contacts by either the least separating speed
					return a->fSeparatingSpeed < b->fSeparatingSpeed;	// Lambda function definition
				}
			);

			// Sort second by depth
			std::sort
			(
				contacts.begin(),	// Beginning of vector
				contacts.end(),		// End of vector
				[]					// [] lambda function is an anonymous function called in-line
				(const ParticleContact* a, const ParticleContact* b)	//  Parameters of Lambda function
				{
					//Sort the particle contacts by either the least separating speed OR the greatest depth
					return a->depth > b->depth;			// Lambda function definition
				}
			);

			// [[2]] Resolve the contact
			//std::cout << contacts[0]->fSeparatingSpeed << " : " << contacts[0]->depth << std::endl;
			contacts[0]->Resolve(time);
			//for (unsigned i = 0; i < contactSize; i++) contacts[i]->Resolve(time);

			// [[3]] Increment resolve count
			current_iterations++;

			// [[4]] if sS of all contacts are >= 0 AND Depth <=0, end
			int check = 0;
			for (unsigned i = 0; i < contactSize; i++)
			{
				if (contacts[i]->fSeparatingSpeed >= 0 && contacts[i]->depth <= 0)
				{
					check++;
				}
			}
			if (check == contactSize) return;
		}
	}
}	