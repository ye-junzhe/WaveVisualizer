#!/bin/bash

set -xe

CFLAGS="-Wall -Wextra $(pkg-config --cflags raylib)"
LIBS="$(pkg-config --libs raylib) $(pkg-config --libs glfw3) -lm -ldl -lpthread"

clang $CFLAGS -o visualizer WaveVisualizer.c $LIBS

./visualizer
