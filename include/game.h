#ifndef GAME_H
#define GAME_H

#include "board.h"

void *game_loop(void *arg);
void gravity(session *currentSession);
void birdJump(session *currentSession);
void waitTime(int milliseconds);

#endif
