#!/bin/bash

set -xe


CFLAGS="-Wall -Wextra $(pkg-config --cflags raylib)"
LIBS="$(pkg-config --libs raylib) -lm -ldl -lpthread"

mkdir -p bin

clang $CFLAGS -o ./bin/libplug.dylib -fPIC -shared ./src/plug.c $LIBS
clang $CFLAGS -o ./bin/visualizer ./src/WaveVisualizer.c $LIBS -L./bin/ -lplug
# clang -o ./bin/fft ./src/fft.c -lm

./bin/visualizer
