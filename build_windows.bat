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
SET compilerFlags=-Iext
SET linkerFlags=user32.lib
SET defines=-D_DEBUG -D_CRT_SECURE_NO_WARNINGS

ECHO "Building game..."
cl %cFilenames% %compilerFlags% -Febin/%assembly%.exe %defines% /link %linkerFlags%