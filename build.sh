#!/bin/bash

mkdir -p build
mkdir -p bin

gcc core/chesscore.c -Icore/ -o build/core.o -c
g++ debug/debug.cpp -Icore/ build/core.o -o bin/debug

