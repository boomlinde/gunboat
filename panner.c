#include <math.h>

#include "panner.h"
#include "types.h"

void panner_tick(struct panner *p)
{
	const value_t in = (p->bus_pan + (2.0 * p->params.pan - 1.0));
	p->out_left = 0.5 * (1.0 + sin(0.5 * M_PI * in));
	p->out_right = 0.5 * (1.0 - sin(0.5 * M_PI * in));
}
