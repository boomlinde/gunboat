#include <math.h>
// #include "misc.h"
#include "folder.h"

double shaper(double level)
{
	return asin(sin(0.5 * M_PI * level)) / (M_PI * 0.5);
}

void folder_tick(struct folder *f)
{
	f->out = shaper(f->bus_a * f->bus_b * f->params.scale * 16.0);
}
