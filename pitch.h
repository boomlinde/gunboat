#ifndef _PITCH_H_
#define _PITCH_H_

#include <math.h>

static double p2f(double pitch, double range)
{
	return range * pow(2.0, pitch / 12.0);
}

#endif /* _PITCH_H_ */
