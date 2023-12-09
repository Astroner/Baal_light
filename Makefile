CC=gcc-13
SOURCES:=$(wildcard src/*.c src/*/*.c)
OBJECTS=$(SOURCES:.c=.o)
HEADERS=headers
LIBNAME=Zeb.h

EXECUTABLE=./start

all: build
	$(EXECUTABLE)

build: $(OBJECTS)
	gcc $(CFLAGS) -o $(EXECUTABLE) $^

.c.o:
	gcc $(CFLAGS) -c -I$(HEADERS) -DZEB_DEBUG -Wall -Wextra -o $@ $<

lib: headers/Zeb.h src/Zeb.c
	cat headers/Zeb.h > $(LIBNAME)
	echo "#if defined(ZEB_IMPLEMENTATION)" >> $(LIBNAME)
	tail -n +3 src/Zeb.c >> $(LIBNAME)
	echo "\n#endif // ZEB_IMPLEMENTATION\n" >> $(LIBNAME)

.PHONY: tests
tests: lib
	$(CC) -o tests.gen -std=c99 -pedantic -Wall -Wextra tests/main.spec.c
	./tests.gen
	rm -f tests.gen

.PHONY: clean
clean:
	rm -f $(EXECUTABLE) $(OBJECTS) $(LIBNAME)