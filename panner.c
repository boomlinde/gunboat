#include <math.h>

#include "panner.h"
#include "types.h"

void panner_tick(struct panner *p)
{
	const value_t in = p->bus_pan + p->params.pan;
	p->out_left = sin(0.5 * M_PI * in);
	p->out_right = sin(0.5 * M_PI * (1.0 - in));
}

void panner_reset(struct panner *p)
{
	p->bus_pan = 0.0;
	p->out_left = 0.0;
	p->out_right = 0.0;
}
