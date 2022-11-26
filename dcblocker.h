#ifndef _DCBLOCKER_H_
#define _DCBLOCKER_H_

#include "types.h"

struct dcblocker_ctrl {
	param_t freq;
};

struct monoblocker {
	value_t in;
	value_t in_1;
	value_t out;
};

struct dcblocker {
	struct dcblocker_ctrl params;

	struct monoblocker left;
	struct monoblocker right;
};

void dcblocker_tick(struct dcblocker *b, value_t rate);
void dcblocker_reset(struct dcblocker *b);

#endif /* _DCBLOCKER_H_ */
