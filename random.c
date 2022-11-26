#include <stdlib.h>

#include "types.h"
#include "misc.h"
#include "random.h"

void random_tick(struct random *r, value_t rate)
{
	value_t freq = p2f(180.0 * r->bus_freq, 1.0);
	r->phase += freq / rate;
	if (r->phase >= 1.0) {
		r->out = 2.0 * ((value_t)rand() / (value_t)RAND_MAX) - 1.0;
		r->phase = unitmod(r->phase);
	}
}
