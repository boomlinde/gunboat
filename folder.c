#include <math.h>

#include "folder.h"
#include "types.h"

static value_t gain(value_t level)
{
	value_t shape = asin(sin(0.5 * M_PI * level)) / (M_PI * 0.5);
	return level == 0.0 ? 0.0 : shape / level;
}

static value_t filter(value_t in, value_t rate, value_t tmp)
{
	value_t p = pow(1.0 - 2.0 * 3000.0 / rate, 2.0);
	return (1.0 - p) * in + p * tmp;
}

void folder_tick(struct folder *f, value_t rate)
{
	value_t in = f->polarity * f->bus_a * f->bus_b * f->params.scale * 16.0;

	f->f1 = filter(gain(in), rate, f->f1);
	f->out = in * f->f1;
}
