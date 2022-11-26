#include "synth.h"
#include "matrix.h"
#include "osc.h"
#include "folder.h"
#include "filter.h"
#include "panner.h"
#include "misc.h"
#include "types.h"
#include "dcblocker.h"
#include "random.h"

void synth_tick(struct synth *s, value_t rate)
{
	s->osc1.bus_pitch = s->m.sinks[sink_osc1_pitch];
	s->osc1.bus_phase_offset = s->m.sinks[sink_osc1_phase_offset];
	s->osc2.bus_pitch = s->m.sinks[sink_osc2_pitch];
	s->osc2.bus_phase_offset = s->m.sinks[sink_osc2_phase_offset];
	s->osc3.bus_pitch = s->m.sinks[sink_osc3_pitch];
	s->osc3.bus_phase_offset = s->m.sinks[sink_osc3_phase_offset];
	s->random.bus_freq = s->m.sinks[sink_random_freq];

	s->folder1.bus_a = s->m.sinks[sink_folder1_a];
	s->folder1.bus_b = s->m.sinks[sink_folder1_b];
	s->folder2.bus_a = s->m.sinks[sink_folder2_a];
	s->folder2.bus_b = s->m.sinks[sink_folder2_b];
	s->filter.bus_cutoff = s->m.sinks[sink_filter_cutoff];
	s->filter.bus_input = s->m.sinks[sink_filter_input];
	s->panner.bus_pan = s->m.sinks[sink_panner];

	s->blocker.left.in = clamp(s->panner.out_left * s->m.sinks[sink_out]);
	s->blocker.right.in = clamp(s->panner.out_right * s->m.sinks[sink_out]);

	dcblocker_tick(&s->blocker, rate);

	s->out_left = s->blocker.left.out;
	s->out_right = s->blocker.right.out;

	osc_tick(&s->osc1, rate);
	osc_tick(&s->osc2, rate);
	osc_tick(&s->osc3, rate);
	random_tick(&s->random, rate);
	filter_tick(&s->filter, rate);
	folder_tick(&s->folder1);
	folder_tick(&s->folder2);
	panner_tick(&s->panner);

	s->m.sources[source_folder1] = s->folder1.out;
	s->m.sources[source_folder2] = s->folder2.out;
	s->m.sources[source_filter_lp] = s->filter.out_lp;
	s->m.sources[source_filter_hp] = s->filter.out_hp;
	s->m.sources[source_filter_bp] = s->filter.out_bp;
	s->m.sources[source_osc1_out] = s->osc1.out;
	s->m.sources[source_osc2_out] = s->osc2.out;
	s->m.sources[source_osc3_out] = s->osc3.out;
	s->m.sources[source_random] = s->random.out;
	s->m.sources[source_out] = s->m.sinks[sink_out];
	s->m.sources[source_unit] = 1.0;

	matrix_tick(&s->m);
}
