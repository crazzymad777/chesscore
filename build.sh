#!/bin/bash

gcc core/chesscore.c -Icore/ -o build/core.o -c
g++ cmd/cmd.cpp -Icore/ build/core.o -o bin/cmd

