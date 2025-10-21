cc = clang

cflags = -Wall -Werror -pedantic -std=c99 -O2
dflags = -Wall -Werror -pedantic -std=c99 -g -O0 -fsanitize=address, -fsanitize=undefined -fno-eliminate-unused-debug-types

sdl_include = $(shell pkg-config --cflags sdl3)
sdl_lib = $(shell pkg-config --libs sdl3)

sdl_ttf_include = $(shell pkg-config --cflags sdl3-ttf)
sdl_ttf_lib = $(shell pkg-config --libs sdl3-ttf)

# Este es solo para buscar los archivos .c en src/
#source = $(wildcard src/*.c)

# Esto hace que el Makefile busque archivos .c en src/ y en todas sus subcarpetas
source = $(wildcard src/**/*.c) $(wildcard src/*.c)

DVAR=

output = bin/render_cpu

build:
	mkdir -p bin
	$(cc)  $(dflags) $(DVAR) $(source) -o $(output) $(sdl_include) $(sdl_lib) $(sdl_ttf_include) $(sdl_ttf_lib) -lm

clean:
	rm -rf bin

run:
	./$(output)

