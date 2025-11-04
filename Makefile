dflags = -Wall -Werror -pedantic -std=c99 -g 
dflags = -Wall-Werror -pedantic -std=c99 -g 
cflags = -Wall -Werror -pedantic -std=c99 -O2

sdl_include = ./deps/sdl/i686-w64-mingw32/include
sdl_lib = ./deps/sdl/i686-w64-mingw32/lib -lSDL3
stb_include ?= ./deps/stb

source = $(wildcard src/*.c) $(wildcard src/**/*.c)
output = bin/render_cpu
DVAR =

build:
	gcc $(cflags) $(DVAR) $(source) -o $(output) -I$(sdl_include) -I$(stb_include) -L$(sdl_lib) -lm
	copy "deps\\sdl\\i686-w64-mingw32\\bin\\SDL3.dll" "bin\\SDL3.dll"
	@echo "Compilo exitosamente!!"

debug:
	gcc $(dflags) $(DVAR) $(source) -o $(output) -I$(sdl_include) -I$(stb_include) -L$(sdl_lib) -lm
	copy "deps\\sdl\\i686-w64-mingw32\\bin\\SDL3.dll" "bin\\SDL3.dll"
	@echo "Debugeo exitoso!!"

run:
	./$(output)