CC=clang
LDFLAGS=-lncurses
CFLAGS=-Wall -O2
TESTFLAGS = -lcheck `pkg-config --libs --cflags check` -Wno-unused-function

SOURCES = $(wildcard src/*.c)

TESTSOURCES = $(wildcard test/*.c) $(filter-out src/main.c, $(SOURCES))

all: test tetris

tetris: ./bin/tetris

test: ./bin/test
	@exec ./bin/test

./bin/tetris: $(SOURCES)
	$(CC) $^  $(CFLAGS) $(LDFLAGS) -o $@

./bin/test: $(TESTSOURCES)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) $(TESTFLAGS) -o $@

.PHONY: clean
clean:
	-rm bin/tetris bin/test
