bin = gunboat
src = $(wildcard *.c)
objects = $(patsubst %.c,%.o, $(src))
libs = sdl2

PKGCONFIG ?= pkg-config

LDFLAGS = -flto -ffast-math
LDLIBS = -lm $(shell $(PKGCONFIG) --libs $(libs))
CFLAGS = -flto -O3 -Wall $(shell $(PKGCONFIG) --cflags $(libs))

$(bin): $(objects)

$(bin).o: $(bin).c font.h synth.h dcblocker.h filter.h folder.h matrix.h misc.h osc.h panner.h random.h types.h

osc.o: osc.c osc.h misc.h types.h

synth.o: osc.h matrix.h synth.h folder.h filter.h panner.h misc.h dcblocker.h random.h types.h

matrix.o: matrix.c matrix.h types.h

folder.o: folder.c folder.h misc.h types.h

filter.o: filter.h misc.h types.h

misc.o: misc.c misc.h types.h

panner.o: panner.c panner.h types.h

dcblocker.o: dcblocker.c dcblocker.h types.h

random.o: random.c random.h misc.h types.h

font.h: font.bin
	xxd -i font.bin > font.h

clean:
	-rm -f $(bin) $(bin).exe $(objects) font.h
