#ifndef ELLIE_PLATFORM_H
#define ELLIE_PLATFORM_H

#include "ellie_common.h"
#include <HandmadeMath.h>

typedef enum mouse_button {
    MouseButton_Left,
    MouseButton_Right,
    MouseButton_Middle,
    MouseButton_Max
} mouse_button;

typedef struct dynamic_library {
    void* Handle;
} dynamic_library;

void OpenDynamicLib(dynamic_library* Library, const char* Path);
void CloseDynamicLib(dynamic_library* Library);
void* LoadFunction(dynamic_library* Library, const char* Name);

b8 IsMouseButtonPressed(mouse_button Button);
hmm_vec2 GetMousePosition();

char* PlatformReadFile(const char* Path, i32* OutputSize);

f32 PlatformGetTime();

#endif
