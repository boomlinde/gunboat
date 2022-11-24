#ifndef _PANNER_H_
#define _PANNER_H_

struct panner_ctrl {
	_Atomic double pan;
};

struct panner {
	struct panner_ctrl params;

	double bus_pan;
	double out_left;
	double out_right;
};

void panner_tick(struct panner *p);

#endif /* _PANNER_H_ */
