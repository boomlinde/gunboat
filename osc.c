#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "osc.h"
#include "misc.h"

void osc_tick(struct osc *o, double rate)
{
	double pitch = 10.0 * 12.0 * (o->params.pitch - 0.5) + 24.0 * (o->params.finetune - 0.5);
	double freq;
	double phase;

	pitch += (12 * 5) * (o->bus_pitch);
	freq = p2f(pitch, 440.0 * o->params.range);
	phase = unitmod(o->phase + o->bus_phase_offset * 2.0);

	o->out = sin(M_PI * 2.0 * phase);
	o->phase = unitmod(o->phase + freq / rate);
}
