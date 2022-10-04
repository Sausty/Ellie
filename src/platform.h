#ifndef ELLIE_PLATFORM_H
#define ELLIE_PLATFORM_H

#include "common.h"

char* ReadFile(const char* Path, i32* OutputSize);
f32 PlatformGetTime();

#endif
