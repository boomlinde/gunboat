#include <math.h>

#include "misc.h"

double unitmod(double v)
{
	return fmod(fmod(v, 1.0) + 1.0, 1.0);
}

double clamp(double v)
{
	return fmax(fmin(v, 1.0), -1.0);
}
