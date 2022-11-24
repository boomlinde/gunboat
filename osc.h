#ifndef _OSC_H_
#define _OSC_H_

#include "types.h"

struct osc_ctrl {
	param_t pitch;
	param_t finetune;
	param_t range;
};

struct osc {
	struct osc_ctrl params;
	value_t phase;

	value_t bus_pitch;
	value_t bus_phase_offset;

	value_t out;
};

void osc_tick(struct osc *o, value_t rate);

#endif /* _OSC_H_ */
