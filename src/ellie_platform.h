#ifndef ELLIE_PLATFORM_H
#define ELLIE_PLATFORM_H

#include "ellie_common.h"

typedef enum mouse_button {
    MouseButton_Left,
    MouseButton_Right,
    MouseButton_Middle,
    MouseButton_Max
} mouse_button;

b8 IsMouseButtonPressed(mouse_button Button);
hmm_vec2 GetMousePosition();
char* ReadFile(const char* Path, i32* OutputSize);
f32 PlatformGetTime();

#endif
