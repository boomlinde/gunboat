#ifndef _FOLDER_H_
#define _FOLDER_H_

struct folder_ctrl {
	_Atomic double scale;
};

struct folder {
	struct folder_ctrl params;
	double bus_a;
	double bus_b;
	double out;
};

void folder_tick(struct folder *f);

#endif /* _FOLDER_H_ */
