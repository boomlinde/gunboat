#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "osc.h"
#include "misc.h"
#include "types.h"

void osc_tick(struct osc *o, value_t rate)
{
	value_t pitch = 10.0 * 12.0 * (o->params.pitch - 0.5) + 24.0 * (o->params.finetune - 0.5);
	value_t freq;
	value_t phase;

	pitch += (12 * 5) * (o->bus_pitch);
	freq = p2f(pitch, 440.0 * o->params.range);
	phase = unitmod(o->phase + o->bus_phase_offset * 2.0);

	o->out = sin(M_PI * 2.0 * phase);
	o->phase = unitmod(o->phase + freq / rate);
}
