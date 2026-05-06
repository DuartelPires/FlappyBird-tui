#include "game.h"
#include "board.h"
#include "display.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern pthread_mutex_t gameSessionLock;

int checkGroundCollision(session *currentSession) {
  if (currentSession->gameBird.y + currentSession->gameBird.height >= BOARD_HEIGHT)
    return 1;
  return 0;
}

void waitTime(int milliseconds) { usleep(milliseconds * 1000); }

void gravity(session *currentSession) {
  if (checkGroundCollision(currentSession)) {
    currentSession->gameBird.alive = 0;
    return;
  }
  setBirdPosition(currentSession, currentSession->gameBird.x,
                  currentSession->gameBird.y + 1);
  setStartingBoard(currentSession);
}

void *game_loop(void *arg) {
  session *currentSession = (session *)arg;

  while (currentSession->gameBird.alive) {
    pthread_mutex_lock(&gameSessionLock);
    gravity(currentSession);
    pthread_mutex_unlock(&gameSessionLock);

    waitTime(250); 
  }
  return NULL;
}