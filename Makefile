CC=gcc
SOURCES:=$(wildcard src/*.c src/*/*.c)
OBJECTS=$(SOURCES:.c=.o)
HEADERS=headers
LIBNAME=Baal_light.h

EXECUTABLE=./start

all: build
	$(EXECUTABLE)

build: $(OBJECTS)
	gcc $(CFLAGS) -o $(EXECUTABLE) $^

.c.o:
	gcc $(CFLAGS) -c -I$(HEADERS) -Wall -Wextra -o $@ $<

lib: headers/Baal.h src/Baal.c
	cat headers/Baal.h > $(LIBNAME)
	echo "#if defined(BAAL_IMPLEMENTATION)" >> $(LIBNAME)
	tail -n +2 src/Baal.c >> $(LIBNAME)
	echo "\n#endif" >> $(LIBNAME)

test: lib
	$(CC) -o test test.c
	./test
	rm -f test

clean:
	rm -f $(EXECUTABLE) $(OBJECTS) $(LIBNAME)