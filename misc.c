#include <math.h>

#include "misc.h"
#include "types.h"

value_t unitmod(value_t v)
{
	return fmod(fmod(v, 1.0) + 1.0, 1.0);
}

value_t clamp(value_t v)
{
	return fmax(fmin(v, 1.0), -1.0);
}

value_t p2f(value_t pitch, value_t range)
{
	return range * pow(2.0, pitch / 12.0);
}
