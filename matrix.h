#ifndef _MATRIX_H_
#define _MATRIX_H_

enum sources {
	source_osc1_out,
	source_osc2_out,
	source_noise,
	source_folder1,
	source_folder2,
	source_filter_lp,
	source_filter_hp,
	source_filter_bp,
	source_unit,
	source_out1,
	source_out2,
	n_sources,
};

enum sinks {
	sink_osc1_pitch,
	sink_osc1_phase_offset,

	sink_osc2_pitch,
	sink_osc2_phase_offset,

	sink_folder1_a,
	sink_folder1_b,
	sink_folder2_a,
	sink_folder2_b,

	sink_filter_cutoff,
	sink_filter_input,

	sink_panner1,
	sink_panner2,

	sink_out1,
	sink_out2,
	n_sinks,
};

struct matrix_ctrl {
	_Atomic double coefficients[n_sources][n_sinks];
};

struct matrix {
	struct matrix_ctrl params;
	char info[n_sources][n_sinks][80];

	double sources[n_sources];
	double sinks[n_sinks];
};

void matrix_tick(struct matrix *m);

extern char *source_names[];
extern char *sink_names[];
extern char *source_abbrev[];
extern char *sink_abbrev[];

#endif /* _MATRIX_H_ */
