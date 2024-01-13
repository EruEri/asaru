OBJECT_SRC=$(foreach file, $(shell ls src/*.c), src/$(shell basename $(file) .c ).o )
FLAGS=-std=c99 -O2 -Wall
DEBUG=
TARGET=asaru
CC=cc
CFLAGS=$(shell pkg-config --cflags libimobiledevice-1.0)
CLIBS=$(shell pkg-config --libs libimobiledevice-1.0)


$(TARGET): $(OBJECT_SRC)
	$(CC) $(FLAGS) $(CFLAGS) $(DEBUG) -o $@ $^ $(CLIBS)

src/%.o: src/%.c
	$(CC) $(FLAGS) $(CFLAGS) $(DEBUG) -c -o $@ $<

clean:
	find . -type f -name "$(TARGET)" -delete
	find . -type f -name '*.o' -delete