#ifndef _OSC_H_
#define _OSC_H_

struct osc_ctrl {
	_Atomic double pitch;
	_Atomic double finetune;
	_Atomic double range;
};

struct osc {
	struct osc_ctrl params;
	double phase;

	double bus_pitch;
	double bus_phase_offset;

	double out;
};

void osc_tick(struct osc *o, double rate);

#endif /* _OSC_H_ */
