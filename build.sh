#!/bin/bash
# Build script for the project
set echo on

mkdir -p bin

# Get a list of all the .c files
cFilenames=$(find . -type f -name "*.c")

assembly="Ellie"
compilerFlags="-g -fPIC"
linkerFlags="-lX11 -lX11-xcb -lxcb -lGL -lGLX -L/usr/X11R6/lib"
defines="-D_DEBUG"

echo "Building game..."
clang $cFilenames $compilerFlags -o bin/$assembly $defines $includeFlags $linkerFlags
