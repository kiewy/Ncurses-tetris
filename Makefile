CC=clang
LDFLAGS=-lncurses
CFLAGS=-Wall -O2
TESTFLAGS = -lcheck `pkg-config --libs --cflags check` -Wno-unused-function

SOURCES = $(wildcard src/*.c)
HFILES  = $(wildcard src/*.h)

TESTSOURCES = $(wildcard test/*.c) $(filter-out src/main.c, $(SOURCES))
TESTHFILES  = $(wildcard test/*.h)

tetris: ./bin/main

test: ./bin/test
	@exec ./bin/test

./bin/main: $(SOURCES)
	$(CC) $^  $(CFLAGS) $(LDFLAGS) -o $@

./bin/test: $(TESTSOURCES)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS) $(TESTFLAGS)

.PHONY: clean
clean:
	-rm bin/main bin/test
