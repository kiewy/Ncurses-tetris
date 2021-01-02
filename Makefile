CC=gcc
LDFLAGS=-lncurses
CFLAGS=-Wall 

src = $(wildcard src/*.c)
obj = $(src: src/%.c=obj/%.o) 

./bin/main: $(obj)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

$(obj): obj/%.o :src/%.c
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)
