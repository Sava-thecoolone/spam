CC = gcc
override CFLAGS += -Wall -Wextra -pedantic

sources = ${wildcard *.c libs/*.c}
objects = ${patsubst %.c,build/%.o,$(sources)}

all: $(objects)
	$(CC) -o build/spam $(objects) $(CFLAGS)

$(objects): build/%.o: %.c
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	rm -f build/spam $(objects)