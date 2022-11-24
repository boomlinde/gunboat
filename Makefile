bin = gunboat
src = $(wildcard *.c)
objects = $(patsubst %.c,%.o, $(src))
libs = sdl2

LDFLAGS = -flto -lm $(shell pkg-config --libs $(libs))
CFLAGS = -flto -O3 -Wall $(shell pkg-config --cflags $(libs))

$(bin): $(objects)

$(bin).o: $(bin).c font.h synth.h

osc.o: osc.c osc.h misc.h

synth.o: osc.h matrix.h synth.h folder.h filter.h panner.h misc.h

matrix.o: matrix.c matrix.h

folder.o: folder.c folder.h

filter.o: filter.h misc.h

misc.o: misc.c misc.h

panner.o: panner.c panner.h

font.h: font.bin
	xxd -i font.bin > font.h

clean:
	-rm -f $(bin) $(objects) font.h
