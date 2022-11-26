#include <math.h>
#include "filter.h"
#include "misc.h"
#include "types.h"

static void halftick(struct filter *f, value_t rate, value_t freq, value_t q)
{
	f->out_lp = clamp(f->out_lp + freq * f->out_bp);
	f->out_hp = clamp(q * clamp(f->bus_input) - f->out_lp - q * f->out_bp);
	f->out_bp = clamp(freq * f->out_hp + f->out_bp);
}

void filter_tick(struct filter *f, value_t rate)
{
	value_t cutoff = fmin(rate/6.0, p2f(128.0 * f->bus_cutoff, 27.5));
	value_t freq = 2.0 * sin(M_PI * cutoff / (rate * 3.0));
	value_t q = 0.99 * (1.0 - f->params.resonance);

	halftick(f, rate * 3.0, freq, q);
	halftick(f, rate * 3.0, freq, q);
	halftick(f, rate * 3.0, freq, q);
}

void filter_reset(struct filter *f)
{
	f->bus_cutoff = 0.0;
	f->bus_input = 0.0;
	f->out_lp = 0.0;
	f->out_hp = 0.0;
	f->out_bp = 0.0;
}
