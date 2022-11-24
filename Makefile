bin = gunboat
src = $(wildcard *.c)
objects = $(patsubst %.c,%.o, $(src))
libs = sdl2

LDFLAGS = -lm $(shell pkg-config --libs $(libs))
CFLAGS = -O3 -Wall $(shell pkg-config --cflags $(libs))

$(bin): $(objects)

$(bin).o: $(bin).c font.h synth.h misc.h

osc.o: osc.c osc.h pitch.h misc.h

synth.o: osc.h matrix.h synth.h folder.h filter.h panner.h misc.h

matrix.o: matrix.c matrix.h

folder.o: folder.c folder.h

filter.o: filter.h pitch.h misc.h

misc.o: misc.c misc.h

panner.o: panner.c panner.h

font.h: font.bin
	xxd -i font.bin > font.h

clean:
	-rm -f $(bin) $(objects) font.h
