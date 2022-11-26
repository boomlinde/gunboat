#ifndef _FOLDER_H_
#define _FOLDER_H_

#include "types.h"

struct folder_ctrl {
	param_t scale;
};

struct folder {
	struct folder_ctrl params;
	value_t polarity;
	value_t bus_a;
	value_t bus_b;
	value_t out;

	value_t f1;

	value_t coefficient;
	value_t rate;
};

void folder_tick(struct folder *f, value_t rate);

#endif /* _FOLDER_H_ */
