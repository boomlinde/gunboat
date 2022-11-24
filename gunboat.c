#include <stdio.h>
#include <time.h>
#include <stddef.h>
#include <SDL.h>
#include <math.h>

#include "font.h"
#include "synth.h"

#define WIDTH 376
#define HEIGHT 248

#define SL_SIZE 24
#define SL_SEP ((SL_SIZE * 4) / 3)

#define MATRIX_SL_SIZE 16
#define MATRIX_SL_SEP (MATRIX_SL_SIZE + 4)
#define MATRIX_X 92
#define MATRIX_Y MATRIX_SL_SEP

#define OSC1_COLOR 0xb02020
#define OSC2_COLOR 0x20b0b0
#define FOLDER_COLOR 0x20b000
#define FILTER_COLOR 0xb020b0
#define PANNER_COLOR 0x8080b0

void text_puts(int x, int y, char *const s);

struct param {
	_Atomic double target;
	_Atomic double *value;
};

struct slider {
	char *abbrev;
	char *info;
	int color;
	int x;
	int y;
	_Atomic double *value;
	struct param ctrl;
	double _default;
};

void slider_draw(struct slider *s, int size);
int slider_hovered(struct slider *s, int x, int y, int size);

void redraw(void);
void update_hovered(SDL_Point mouse);

void audio_cb(void *data, Uint8 *stream, int len);

SDL_Window *w = NULL;
SDL_Renderer *r = NULL;
struct slider *held = NULL;
struct slider *hovered = NULL;
SDL_AudioSpec audio_want = {
	.freq = 48000,
	.format = AUDIO_F32,
	.channels = 2,
	.callback = audio_cb,
	.samples = 1024,
};
SDL_AudioSpec audiospec = {0};

void param_smooth(struct param *p, double rate);

struct synth s = {
	.osc1 = { .params = { .pitch = 0.5, .finetune = 0.5, .range = 1.0, }, },
	.osc2 = { .params = { .pitch = 0.5, .finetune = 0.5, .range = 1.0, }, },
	.folder1 = { .params = { .scale = 1.0/16.0 }, },
	.folder2 = { .params = { .scale = 1.0/16.0 }, },
	.filter = { .params = { .resonance = 0.5, }, },
	.panner1 = { .params = { .pan = 0.5, }, },
	.panner2 = { .params = { .pan = 0.5, }, },
};

_Atomic double amp = 0.0;
struct slider sliders[] = {
	{"~1", "pitch (oscillator 1)", OSC1_COLOR, 8, 20, &s.osc1.params.pitch},
	{":1", "fine tune (oscillator 1)", OSC1_COLOR, 8, 52, &s.osc1.params.finetune},
	{"^1", "range (oscillator 1)", OSC1_COLOR, 8, 84, &s.osc1.params.range},

	{"~2", "pitch (oscillator 2)", OSC2_COLOR, 40, 20, &s.osc2.params.pitch},
	{":2", "fine tune (oscillator 2)", OSC2_COLOR, 40, 52, &s.osc2.params.finetune},
	{"^2", "range (oscillator 2)", OSC2_COLOR, 40, 84, &s.osc2.params.range},

	{"*1", "folding multiplier 1 scale", FOLDER_COLOR, 8, 116, &s.folder1.params.scale},
	{"*2", "folding multiplier 2 scale", FOLDER_COLOR, 40,116, &s.folder2.params.scale},

	{"p1", "panner 1", PANNER_COLOR, 8, 148, &s.panner1.params.pan},
	{"p2", "panner 2", PANNER_COLOR, 40, 148, &s.panner2.params.pan},

	{"fr", "filter resonance", FILTER_COLOR, 24, 180, &s.filter.params.resonance},
};

struct slider matrix_sliders[n_sources][n_sinks];

int nsliders = sizeof(sliders) / sizeof(sliders[0]);

int main(int argc, char **argv)
{
	int i;
	int j;

	SDL_Point mouse = {0};
	SDL_Event e;
	SDL_Point grabpoint = {0};
	SDL_AudioDeviceID dev = 0;

	srand(time(NULL));

	for (i = 0; i < nsliders; i++) {
		sliders[i].ctrl.target = *sliders[i].value;
		sliders[i].ctrl.value = sliders[i].value;
		sliders[i]._default = *sliders[i].value;
	}

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_AUDIO)) {
		printf("failed to init SDL: %s\n", SDL_GetError());
		return 1;
	}

	for (i = 0; i < n_sources; i++) {
		for (j = 0; j < n_sinks; j++) {
			int src_color = i * 128 / n_sources;
			int snk_color = j * 200 / n_sinks;
			int color = 0x60 | (src_color << 8) | (snk_color << 16);
			sprintf(s.m.info[i][j], "%s->%s", source_names[i], sink_names[j]);
			matrix_sliders[i][j].abbrev = "";
			matrix_sliders[i][j].info = s.m.info[i][j];
			matrix_sliders[i][j].color = color;
			matrix_sliders[i][j].value = &s.m.params.coefficients[i][j];
			matrix_sliders[i][j].ctrl.target = s.m.params.coefficients[i][j];
			matrix_sliders[i][j].ctrl.value = &s.m.params.coefficients[i][j];
			matrix_sliders[i][j].x = MATRIX_X + j * MATRIX_SL_SEP;
			matrix_sliders[i][j].y = MATRIX_Y + i * MATRIX_SL_SEP;
		}
	}

	w = SDL_CreateWindow(
			"gunboat",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			2*WIDTH, 2*HEIGHT,
			SDL_WINDOW_RESIZABLE);
	if (!w) goto done;

	r = SDL_CreateRenderer(
			w, -1,
			SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if (!r) goto done;

	dev = SDL_OpenAudioDevice(NULL, 0, &audio_want, &audiospec,
			SDL_AUDIO_ALLOW_FREQUENCY_CHANGE
			|SDL_AUDIO_ALLOW_SAMPLES_CHANGE);

	if (!dev) {
		printf("failed to open audio device: %s\n", SDL_GetError());
		goto done;
	}

	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(r, WIDTH, HEIGHT);
	SDL_RenderSetIntegerScale(r, SDL_TRUE);
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SCALING, "1");
	SDL_PauseAudioDevice(dev, 0);

	redraw();

	for (;;) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				goto done;
				break;
			case SDL_MOUSEBUTTONDOWN:
				update_hovered(mouse);
				if (!hovered) break;

				switch(e.button.button) {
				case SDL_BUTTON_LEFT:
					held = hovered;
					SDL_GetMouseState(&grabpoint.x, &grabpoint.y);
					SDL_SetRelativeMouseMode(SDL_TRUE);
					break;
				case SDL_BUTTON_RIGHT:
					hovered->ctrl.target = hovered->_default;
					break;
				case SDL_BUTTON_MIDDLE:
					hovered->ctrl.target = 1.0;
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (e.button.button != SDL_BUTTON_LEFT) break;
				if (held) {
					held = NULL;
					SDL_WarpMouseInWindow(w, grabpoint.x, grabpoint.y);
					SDL_SetRelativeMouseMode(SDL_FALSE);
					update_hovered(mouse);
				}
				break;
			case SDL_MOUSEMOTION:
				if (!held) {
					mouse.x = e.motion.x;
					mouse.y = e.motion.y;
					update_hovered(mouse);
				} else {
					held->ctrl.target = fmax(0.0, fmin(1.0, (held->ctrl.target - e.motion.yrel * 0.002)));
				}
				break;
			case SDL_KEYDOWN:
				if (!hovered) break;

				switch (e.key.keysym.sym) {
				case SDLK_BACKQUOTE:
					hovered->ctrl.target = 0.0;
					break;
				case SDLK_1:
					hovered->ctrl.target = 0.1;
					break;
				case SDLK_2:
					hovered->ctrl.target = 0.2;
					break;
				case SDLK_3:
					hovered->ctrl.target = 0.3;
					break;
				case SDLK_4:
					hovered->ctrl.target = 0.4;
					break;
				case SDLK_5:
					hovered->ctrl.target = 0.5;
					break;
				case SDLK_6:
					hovered->ctrl.target = 0.6;
					break;
				case SDLK_7:
					hovered->ctrl.target = 0.7;
					break;
				case SDLK_8:
					hovered->ctrl.target = 0.8;
					break;
				case SDLK_9:
					hovered->ctrl.target = 0.9;
					break;
				case SDLK_0:
					hovered->ctrl.target = 1.0;
					break;
				}
				break;
			case SDL_WINDOWEVENT:
				break;
			}
		}

		redraw();
	}

done:
	if (dev) SDL_CloseAudioDevice(dev);
	if (r) SDL_DestroyRenderer(r);
	if (w) SDL_DestroyWindow(w);
	SDL_Quit();
	return 0;
}

void update_hovered(SDL_Point mouse)
{
	int i, j;
	hovered = NULL;
	for (i = 0; i < nsliders; i++) {
		if (slider_hovered(&sliders[i], mouse.x, mouse.y, SL_SIZE)) {
			hovered = &sliders[i];
			return;
		}
	}
	for (i = 0; i < n_sources; i++) {
		for (j = 0; j < n_sinks; j++) {
			if (slider_hovered(&matrix_sliders[i][j], mouse.x, mouse.y, MATRIX_SL_SIZE)) {
				hovered = &matrix_sliders[i][j];
				return;
			}
		}
	}
}

void slider_draw(struct slider *s, int size)
{
	int h = ceil(size * s->ctrl.target);
	int offset = size - h;
	SDL_Rect bg = { s->x, s->y, size, size };
	SDL_Rect fg = { s->x, s->y + offset, size, h };

	SDL_SetRenderDrawColor(r, 0xd0, 0xc0, 0xa0, 0xff);
	SDL_RenderFillRect(r, &bg);
	SDL_SetRenderDrawColor(r, (s->color >> 16) & 0xff, (s->color >> 8) & 0xff, s->color & 0xff, 0xff);
	SDL_RenderFillRect(r, &fg);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0xff);
	text_puts(s->x, s->y, s->abbrev);
}

int slider_hovered(struct slider *s, int x, int y, int size)
{
	return !(x < s->x || x >= s->x + size || y < s->y || y >= s->y + size);
}

void bigslider(struct slider *s) {
	int h = HEIGHT * s->ctrl.target;
	int offset = HEIGHT - h;
	SDL_Rect bg = { 0, 0, WIDTH, HEIGHT };
	SDL_Rect fg = { 0, 0 + offset, WIDTH, h };

	SDL_SetRenderDrawColor(r, 0xd0, 0xc0, 0xa0, 0xff);
	SDL_RenderFillRect(r, &bg);
	SDL_SetRenderDrawColor(r, (s->color >> 16) & 0xff, (s->color >> 8) & 0xff, s->color & 0xff, 0xff);
	SDL_RenderFillRect(r, &fg);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0xff);
	text_puts(8, 8, s->info);
};

void redraw(void)
{
	int i;
	int j;

	SDL_Rect panel = { 0, 0, WIDTH, HEIGHT };

	SDL_SetRenderDrawColor(r, 0, 0, 0, 0xff);
	SDL_RenderClear(r);

	SDL_SetRenderDrawColor(r, 0xf0, 0xe0, 0xd0, 0xff);
	SDL_RenderFillRect(r, &panel);

	if (held) bigslider(held);
	else {
		for (i = 0; i < n_sources; i++) {
			SDL_SetRenderDrawColor(r, 0, 0, 0, 0xff);
			text_puts(MATRIX_X - 24 + 4, MATRIX_Y + 4 + i * MATRIX_SL_SEP, source_abbrev[i]);
		}
		for (i = 0; i < n_sinks; i++) {
			SDL_SetRenderDrawColor(r, 0, 0, 0, 0xff);
			text_puts(MATRIX_X + i * MATRIX_SL_SEP, MATRIX_Y - 16 + 4, sink_abbrev[i]);
		}
		for (i = 0; i < nsliders; i++) slider_draw(&sliders[i], 24);
		for (i = 0; i < n_sources; i++) {
			for (j = 0; j < n_sinks; j++) {
				slider_draw(&matrix_sliders[i][j], MATRIX_SL_SIZE);
			}
		}
		if (hovered) {
			text_puts(0, HEIGHT - 8, hovered->info);
		}
	}
	SDL_RenderPresent(r);
}

void audio_cb(void *data, Uint8 *stream, int len)
{
	int i, j, k;
	const double r = audiospec.freq;
	const double a = exp(-2 * M_PI / (40.0 * 0.001 * r));
	float *buf = (float *)stream;

	for (i = 0; i < len / sizeof(buf[0]); i += 2) {
		synth_tick(&s, r);

		buf[i] = s.out_left;
		buf[i + 1] = s.out_right;

		for (j = 0; j < nsliders; j++) {
			param_smooth(&sliders[j].ctrl, a);
		}
		for (j = 0; j < n_sources; j++) {
			for (k = 0; k < n_sinks; k++) {
				param_smooth(&matrix_sliders[j][k].ctrl, a);
			}
		}
	}
}

void param_smooth(struct param *p, double a)
{
	double value = *p->value;
	double target = p->target;
	double b = 1.0 - a;
	*p->value = target * b + value * a;
}

void text_putc(int x, int y, uint8_t ch)
{
	int d;
	size_t fontidx = ch * 8;

	for (d = 0; d < 8; d++) {
		if (font_bin[fontidx + d] & 0x80) SDL_RenderDrawPoint(r, x + 0, y + d);
		if (font_bin[fontidx + d] & 0x40) SDL_RenderDrawPoint(r, x + 1, y + d);
		if (font_bin[fontidx + d] & 0x20) SDL_RenderDrawPoint(r, x + 2, y + d);
		if (font_bin[fontidx + d] & 0x10) SDL_RenderDrawPoint(r, x + 3, y + d);
		if (font_bin[fontidx + d] & 0x08) SDL_RenderDrawPoint(r, x + 4, y + d);
		if (font_bin[fontidx + d] & 0x04) SDL_RenderDrawPoint(r, x + 5, y + d);
		if (font_bin[fontidx + d] & 0x02) SDL_RenderDrawPoint(r, x + 6, y + d);
		if (font_bin[fontidx + d] & 0x01) SDL_RenderDrawPoint(r, x + 7, y + d);
	}
}

void text_puts(int x, int y, char *const s)
{
	int ox = x;
	int i;
	for (i = 0; s[i] != 0; i++) {
		char ch = s[i];
		if (ch == '\n') {
			y += 1;
			x = ox;
		} else {
			text_putc(x, y, ch);
			x += 8;
		}
	}
}
