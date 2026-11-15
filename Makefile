CC = gcc
override CFLAGS += -Wall -Wextra -pedantic

sources = ${wildcard *.c libs/*.c}
objects = ${patsubst %.c, build/%.o, $(sources)}

all: $(objects)
	$(CC) -o main $(objects) $(CFLAGS)

build/%.o: %.c
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	rm -f $(objects)