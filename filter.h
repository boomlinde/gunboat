#ifndef _FILTER_H_
#define _FILTER_H_

struct filter_ctrl {
	_Atomic double resonance;
};
struct filter {
	struct filter_ctrl params;

	double bus_cutoff;
	double bus_input;

	double out_lp;
	double out_hp;
	double out_bp;
};

void filter_tick(struct filter *f, double rate);

#endif /* _FILTER_H_ */
