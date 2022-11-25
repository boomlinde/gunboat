bin = gunboat
src = $(wildcard *.c)
objects = $(patsubst %.c,%.o, $(src))
libs = sdl2

PKGCONFIG ?= pkg-config

LDFLAGS = -flto -ffast-math -lm $(shell $(PKGCONFIG) --libs $(libs))
CFLAGS = -flto -O3 -Wall $(shell $(PKGCONFIG) --cflags $(libs))

$(bin): $(objects)

$(bin).o: $(bin).c font.h synth.h types.h

osc.o: osc.c osc.h misc.h types.h

synth.o: osc.h matrix.h synth.h folder.h filter.h panner.h misc.h dcblocker.h types.h

matrix.o: matrix.c matrix.h types.h

folder.o: folder.c folder.h types.h

filter.o: filter.h misc.h types.h

misc.o: misc.c misc.h types.h

panner.o: panner.c panner.h types.h

dcblocker.o: dcblocker.c dcblocker.h types.h

font.h: font.bin
	xxd -i font.bin > font.h

clean:
	-rm -f $(bin) $(bin).exe $(objects) font.h
