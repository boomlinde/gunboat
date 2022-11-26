#ifndef _PANNER_H_
#define _PANNER_H_

#include "types.h"

struct panner_ctrl {
	param_t pan;
};

struct panner {
	struct panner_ctrl params;

	value_t bus_pan;
	value_t out_left;
	value_t out_right;
};

void panner_tick(struct panner *p);
void panner_reset(struct panner *p);

#endif /* _PANNER_H_ */
