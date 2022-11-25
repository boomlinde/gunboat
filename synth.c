#include <stdlib.h>

#include "synth.h"
#include "matrix.h"
#include "osc.h"
#include "folder.h"
#include "filter.h"
#include "panner.h"
#include "misc.h"
#include "types.h"
#include "dcblocker.h"

void synth_tick(struct synth *s, value_t rate)
{
	s->m.sources[source_noise] = 2.0 * ((value_t)rand() / (value_t)RAND_MAX) - 1.0;

	s->osc1.bus_pitch = s->m.sinks[sink_osc1_pitch];
	s->osc1.bus_phase_offset = s->m.sinks[sink_osc1_phase_offset];

	s->osc2.bus_pitch = s->m.sinks[sink_osc2_pitch];
	s->osc2.bus_phase_offset = s->m.sinks[sink_osc2_phase_offset];
	s->folder1.bus_a = s->m.sinks[sink_folder1_a];
	s->folder1.bus_b = s->m.sinks[sink_folder1_b];
	s->folder2.bus_a = s->m.sinks[sink_folder2_a];
	s->folder2.bus_b = s->m.sinks[sink_folder2_b];
	s->filter.bus_cutoff = s->m.sinks[sink_filter_cutoff];
	s->filter.bus_input = s->m.sinks[sink_filter_input];
	s->panner1.bus_pan = s->m.sinks[sink_panner1];
	s->panner2.bus_pan = s->m.sinks[sink_panner2];

	s->blocker.left.in = clamp(s->panner1.out_left * s->m.sinks[sink_out1] + s->panner2.out_left * s->m.sinks[sink_out2]);
	s->blocker.right.in = clamp(s->panner1.out_right * s->m.sinks[sink_out1] + s->panner2.out_right * s->m.sinks[sink_out2]);

	dcblocker_tick(&s->blocker, rate);

	s->out_left = s->blocker.left.out;
	s->out_right = s->blocker.right.out;

	osc_tick(&s->osc1, rate);
	osc_tick(&s->osc2, rate);
	filter_tick(&s->filter, rate);
	folder_tick(&s->folder1);
	folder_tick(&s->folder2);
	panner_tick(&s->panner1);
	panner_tick(&s->panner2);

	s->m.sources[source_folder1] = s->folder1.out;
	s->m.sources[source_folder2] = s->folder2.out;
	s->m.sources[source_filter_lp] = s->filter.out_lp;
	s->m.sources[source_filter_hp] = s->filter.out_hp;
	s->m.sources[source_filter_bp] = s->filter.out_bp;
	s->m.sources[source_osc1_out] = s->osc1.out;
	s->m.sources[source_osc2_out] = s->osc2.out;
	s->m.sources[source_out1] = s->m.sinks[sink_out1];
	s->m.sources[source_out2] = s->m.sinks[sink_out2];
	s->m.sources[source_unit] = 1.0;

	matrix_tick(&s->m);
}
