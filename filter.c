#include <math.h>
#include "filter.h"
#include "misc.h"
#include "types.h"

void filter_tick(struct filter *f, value_t rate)
{
	value_t cutoff = fmin(rate/4.0, p2f(128.0 * f->bus_cutoff, 27.5));
	value_t freq = 2.0 * sin (M_PI * cutoff / rate);
	value_t q = 0.99 * (1.0 - f->params.resonance);
	value_t scale = q;

	f->out_lp = clamp(f->out_lp + freq * f->out_bp);
	f->out_hp = clamp(scale * clamp(f->bus_input) - f->out_lp - q * f->out_bp);
	f->out_bp = clamp(freq * f->out_hp + f->out_bp);
}
