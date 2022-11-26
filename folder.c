#include <math.h>

#include "folder.h"
#include "misc.h"
#include "types.h"

static value_t gain(value_t level)
{
	value_t shape = asin(sin(0.5 * M_PI * level)) / (M_PI * 0.5);
	return level == 0.0 ? 0.0 : shape / level;
}

static value_t filter(value_t in, value_t rate, value_t tmp)
{
	value_t p = 1.0 - 2.0 * 3000.0 / rate;
	return (1.0 - p * p) * clamp(in) + p * p * tmp;
}

void folder_tick(struct folder *f, value_t rate)
{
	value_t in = f->polarity * f->bus_a * f->bus_b * f->params.scale * 16.0;

	f->f1 = filter(gain(in), rate * 2.0, f->f1);
	f->f1 += filter(gain(in), rate * 2.0, f->f1);
	f->f1 /= 2.0;
	f->out = clamp(in * f->f1);
}

void folder_reset(struct folder *f)
{
	f->bus_a = 0.0;
	f->bus_b = 0.0;
	f->out = 0.0;
	f->f1 = 0.0;
}
