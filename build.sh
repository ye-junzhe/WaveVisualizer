#!/bin/bash

set -xe


CFLAGS="-Wall -Wextra -ggdb $(pkg-config --cflags raylib)"
LIBS="$(pkg-config --libs raylib) -lm -ldl -lpthread"

mkdir -p bin

if [ -n "${HOTRELOAD}" ]; then
    clang $CFLAGS -dynamiclib -o ./bin/libplug.dylib -fPIC -shared ./src/plug.c $LIBS
    clang $CFLAGS-DHOTRELOAD -o ./bin/WaveVisualizer ./src/WaveVisualizer.c $LIBS -L./bin/
else
    clang $CFLAGS  -o ./bin/WaveVisualizer ./src/plug.c ./src/WaveVisualizer.c $LIBS -L./bin/
fi
# clang -o ./bin/fft ./src/fft.c -lm
