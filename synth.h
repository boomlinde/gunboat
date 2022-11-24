#ifndef _SYNTH_H_
#define _SYNTH_H_

#include "matrix.h"
#include "osc.h"
#include "folder.h"
#include "filter.h"
#include "panner.h"
#include "types.h"

struct synth {
	struct osc osc1;
	struct osc osc2;
	struct folder folder1;
	struct folder folder2;
	struct filter filter;
	struct panner panner1;
	struct panner panner2;

	struct matrix m;

	value_t out_left;
	value_t out_right;
};

void synth_tick(struct synth *s, value_t rate);

#endif /* _SYNTH_H_ */
