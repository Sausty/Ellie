@ECHO OFF
SetLocal EnableDelayedExpansion

IF NOT EXIST bin (
	mkdir bin
)

SET cFilenames=
FOR /R %%f in (*.c) do (
    SET cFilenames=!cFilenames! %%f
)

SET assembly=Ellie
SET compilerFlags=-g -Iext
SET linkerFlags=-luser32
SET defines=-D_DEBUG -D_CRT_SECURE_NO_WARNINGS

ECHO "Building game..."
clang %cFilenames% %compilerFlags% -o bin/%assembly%.exe %defines% %linkerFlags%