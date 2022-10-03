#ifndef ELLIE_GAME_H
#define ELLIE_GAME_H

#include "common.h"

void GameInit();
void GameUpdate(f32 DeltaTime);
void GameExit();
void GameResize(u32 Width, u32 Height);

#endif
