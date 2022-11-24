#ifndef _FILTER_H_
#define _FILTER_H_

#include "types.h"

struct filter_ctrl {
	param_t resonance;
};

struct filter {
	struct filter_ctrl params;

	value_t bus_cutoff;
	value_t bus_input;

	value_t out_lp;
	value_t out_hp;
	value_t out_bp;
};

void filter_tick(struct filter *f, value_t rate);

#endif /* _FILTER_H_ */
