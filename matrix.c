#include "matrix.h"

char *source_names[] = {
	[source_osc1_out] = "osc1",
	[source_osc2_out] = "osc2",
	[source_osc3_out] = "osc3",
	[source_random] = "clocked random",
	[source_folder1] = "folding multiplier 1",
	[source_folder2] = "folding multiplier 2",
	[source_filter_lp] = "lowpass filter",
	[source_filter_hp] = "highpass filter",
	[source_filter_bp] = "bandpass filter",
	[source_unit] = "unit",
	[source_out] = "audio output feedback",
};

char *sink_names[] = {
	[sink_osc1_pitch] = "osc1 pitch",
	[sink_osc1_phase_offset] = "osc1 phase",
	[sink_osc2_pitch] = "osc2 pitch",
	[sink_osc2_phase_offset] = "osc2 phase",
	[sink_osc3_pitch] = "osc3 pitch",
	[sink_osc3_phase_offset] = "osc3 phase",
	[sink_random_freq] = "random clock speed",
	[sink_folder1_a] = "folding multiplier 1A",
	[sink_folder1_b] = "folding multiplier 1B",
	[sink_folder2_a] = "folding multiplier 2A",
	[sink_folder2_b] = "folding multiplier 2B",
	[sink_filter_cutoff] = "filter cutoff",
	[sink_filter_input] = "filter input",
	[sink_panner] = "output panning",
	[sink_out] = "audio output",
};

char *source_abbrev[] = {
	[source_osc1_out] = "~1",
	[source_osc2_out] = "~2",
	[source_osc3_out] = "~3",
	[source_random] = "##",
	[source_folder1] = "*1",
	[source_folder2] = "*2",
	[source_filter_lp] = "lp",
	[source_filter_hp] = "hp",
	[source_filter_bp] = "bp",
	[source_unit] = "+1",
	[source_out] = "<>",
};

char *sink_abbrev[] = {
	[sink_osc1_pitch] = "~1",
	[sink_osc1_phase_offset] = "/1",
	[sink_osc2_pitch] = "~2",
	[sink_osc2_phase_offset] = "/2",
	[sink_osc3_pitch] = "~3",
	[sink_osc3_phase_offset] = "/3",
	[sink_random_freq] = "#~",
	[sink_folder1_a] = "a1",
	[sink_folder1_b] = "b1",
	[sink_folder2_a] = "a2",
	[sink_folder2_b] = "b2",
	[sink_filter_cutoff] = "/f",
	[sink_filter_input] = ">f",
	[sink_panner] = "><",
	[sink_out] = "<>",
};

void matrix_tick(struct matrix *m)
{
	int i;
	int j;

	for (i = 0; i < n_sinks; i++) {
		m->sinks[i] = 0;
		for (j = 0; j < n_sources; j++) {
			m->sinks[i] += m->sources[j] * m->params.coefficients[j][i];
		}
	}
}

void matrix_reset(struct matrix *m)
{
	int i;
	for (i = 0; i < n_sources; i++) {
		m->sources[i] = 0.0;
	}
	for (i = 0; i < n_sinks; i++) {
		m->sinks[i] = 0.0;
	}
}
