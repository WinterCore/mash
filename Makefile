OS := $(shell uname)
CFLAGS = -std=c11 -Wall -Wextra

.PHONY: clean debug all

release: CFLAGS += -DNDEBUG
release: Out

debug: CFLAGS += -ggdb -O0 -DDEBUG
debug: Out

SRC_C=$(shell find . -name "*.c")
SRC_ALL=$(shell find . -name "*.c" -o -name '*.h')

Out: $(SRC_ALL)
	cc $(CFLAGS) -o Out $(SRC_C) $(LDFLAGS)

all: Out

clean:
	rm -rf Out ./src/*.o ./src/**/*.o ./Out.dSYM
