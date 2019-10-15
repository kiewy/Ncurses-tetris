CC=gcc
CFLAGS=-Wall -lcurses

src = $(wildcard src/*.c)
obj = $(src: src/%.c=obj/%.o) 

./bin/main: $(obj)
	$(CC) $^ -o $@ $(CFLAGS)

$(obj): obj/%.o :src/%.c
	$(CC) $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) myprog
