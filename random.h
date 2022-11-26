#ifndef _RANDOM_H_
#define _RANDOM_H_

#include "types.h"

struct random {
	value_t phase;
	value_t bus_freq;
	value_t out;
};

void random_tick(struct random *r, value_t rate);
void random_reset(struct random *r);

#endif /* _RANDOM_H_ */
