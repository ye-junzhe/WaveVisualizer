#!/bin/bash

set -xe


CFLAGS="-Wall -Wextra -ggdb $(pkg-config --cflags raylib)"
LIBS="$(pkg-config --libs raylib) -lm -ldl -lpthread"

mkdir -p bin

clang $CFLAGS -dynamiclib -g -o ./bin/libplug.dylib -fPIC -shared ./src/plug.c $LIBS
clang $CFLAGS -g -o ./bin/visualizer ./src/plug.c ./src/WaveVisualizer.c $LIBS -L./bin/ -L./bin/
# clang -o ./bin/fft ./src/fft.c -lm
