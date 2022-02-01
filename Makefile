CC=clang
LDFLAGS=-lncurses
CFLAGS=-Wall -O3
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

install:
	#install scores
	#install -d $(DESTDIR)/usr/share/ntris
	# install executable
	install -d $(DESTDIR)/usr/bin/
	install -m 755 ./bin/tetris $(DESTDIR)/usr/bin/ntris

.PHONY: clean
clean:
	-rm bin/tetris bin/test
