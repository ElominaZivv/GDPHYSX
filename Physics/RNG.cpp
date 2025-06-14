#include "RNG.h"

float rngFloat(float inclusive_min, float inclusive_max)
{
	//		Lower Bound		+	(Difference of Max and Min			*		random num [0.0, 1.0] = (0->RAND_MAX)/RAND_MAX)
	return	inclusive_min	+	((inclusive_max-inclusive_min)		*		((float)rand()/RAND_MAX));
}