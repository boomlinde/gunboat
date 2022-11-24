#include <math.h>
#include "filter.h"
#include "misc.h"

void filter_tick(struct filter *f, double rate)
{
	double cutoff = fmin(rate/4.0, p2f(128.0 * f->bus_cutoff, 27.5));
	double freq = 2.0 * sin (M_PI * cutoff / rate);
	double q = 0.99 * (1.0 - f->params.resonance);
	double scale = q;

	f->out_lp = clamp(f->out_lp + freq * f->out_bp);
	f->out_hp = clamp(scale * clamp(f->bus_input) - f->out_lp - q * f->out_bp);
	f->out_bp = clamp(freq * f->out_hp + f->out_bp);
}
