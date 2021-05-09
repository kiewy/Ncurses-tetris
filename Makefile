CC=clang
LDFLAGS=-lncurses
CFLAGS=-Wall -O2
TESTFLAGS = -lcheck `pkg-config --libs --cflags check`

SOURCES = $(wildcard src/*.c)
HFILES  = $(wildcard src/*.h)

TESTSOURCES = $(wildcard test/*.c) $(filter-out src/main.c, $(SOURCES))
TESTHFILES  = $(wildcard test/*.h)

tetris: ./bin/main

test: ./bin/test
	@exec ./bin/test

./bin/main: $(SOURCES)
	@echo "MAIN: COMPILING WITH: "
	@echo " --  all: $^"
	$(CC) $^  $(CFLAGS) $(LDFLAGS) -o $@

./bin/test: $(TESTSOURCES)
	@echo "TEST: compiling with: "
	@echo " --  all: $^"
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS) $(TESTFLAGS)
