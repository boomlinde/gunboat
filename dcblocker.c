#include <math.h>

#include "types.h"
#include "dcblocker.h"

static void monoblocker_tick(struct monoblocker *b, value_t r)
{
	b->out = b->in - b->in_1 + r * b->out;
	b->in_1 = b->in;
}

void dcblocker_tick(struct dcblocker *b, value_t rate)
{
	value_t freq = 5.0 + b->params.freq * 95.0;
	value_t r = 1.0 - 2.0 * M_PI * (freq / rate);
	monoblocker_tick(&b->left, r);
	monoblocker_tick(&b->right, r);
}
