#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "types.h"

enum sources {
	source_osc1_out,
	source_osc2_out,
	source_osc3_out,
	source_random,
	source_folder1,
	source_folder2,
	source_filter_lp,
	source_filter_hp,
	source_filter_bp,
	source_unit,
	source_out,
	n_sources,
};

enum sinks {
	sink_osc1_pitch,
	sink_osc1_phase_offset,

	sink_osc2_pitch,
	sink_osc2_phase_offset,

	sink_osc3_pitch,
	sink_osc3_phase_offset,

	sink_random_freq,

	sink_folder1_a,
	sink_folder1_b,
	sink_folder2_a,
	sink_folder2_b,

	sink_filter_cutoff,
	sink_filter_input,

	sink_panner,

	sink_out,
	n_sinks,
};

struct matrix_ctrl {
	param_t coefficients[n_sources][n_sinks];
};

struct matrix {
	struct matrix_ctrl params;
	char info[n_sources][n_sinks][80];

	value_t sources[n_sources];
	value_t sinks[n_sinks];
};

void matrix_tick(struct matrix *m);
void matrix_reset(struct matrix *m);

extern char *source_names[];
extern char *sink_names[];
extern char *source_abbrev[];
extern char *sink_abbrev[];

#endif /* _MATRIX_H_ */
