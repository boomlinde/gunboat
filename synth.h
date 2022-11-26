#ifndef _SYNTH_H_
#define _SYNTH_H_

#include "matrix.h"
#include "osc.h"
#include "folder.h"
#include "filter.h"
#include "panner.h"
#include "dcblocker.h"
#include "types.h"
#include "random.h"

struct synth {
	struct osc osc1;
	struct osc osc2;
	struct osc osc3;
	struct random random;
	struct folder folder1;
	struct folder folder2;
	struct filter filter;
	struct panner panner;
	struct dcblocker blocker;

	struct matrix m;

	param_t volume;

	value_t out_left;
	value_t out_right;
};

void synth_tick(struct synth *s, value_t rate);
void synth_reset(struct synth *s);

#endif /* _SYNTH_H_ */
