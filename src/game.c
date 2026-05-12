#include "game.h"
#include "board.h"
#include "display.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern pthread_mutex_t gameSessionLock;

int checkGroundCollision(session *currentSession) {
  if (currentSession->gameBird.y + currentSession->gameBird.height >=
      BOARD_HEIGHT)
    return 1;
  return 0;
}

void birdJump(session *currentSession) {
  if (currentSession->gameBird.y > 0) {
    currentSession->gameBird.y -= 3; // Jump!
    if (currentSession->gameBird.y < 0)
      currentSession->gameBird.y = 0;
    setStartingBoard(currentSession);
  }
}

void waitTime(int milliseconds) { usleep(milliseconds * 1000); }

// if you didnt hit the ground, you fall
void gravity(session *currentSession) {
  if (checkGroundCollision(currentSession)) {
    currentSession->gameBird.alive = 0;
    return;
  }
  setBirdPosition(currentSession, currentSession->gameBird.x,
                  currentSession->gameBird.y + 1);
  setStartingBoard(currentSession);
}

void deathAnimation(session *currentSession) {
  // Continue falling until hitting the ground

  setBirdPosition(currentSession, currentSession->gameBird.x - 1,
                  currentSession->gameBird.y);
  while (!checkGroundCollision(currentSession)) {
    pthread_mutex_lock(&gameSessionLock);
    setBirdPosition(currentSession, currentSession->gameBird.x,
                    currentSession->gameBird.y + 1);
    setStartingBoard(currentSession);
    draw_board(currentSession);
    pthread_mutex_unlock(&gameSessionLock);
    waitTime(100);
  }
}

void *game_loop(void *arg) {
  session *currentSession = (session *)arg;

  while (currentSession->gameBird.alive) {
    pthread_mutex_lock(&gameSessionLock);
    gravity(currentSession);
    pthread_mutex_unlock(&gameSessionLock);

    waitTime(150);
  }

  deathAnimation(currentSession);

  return NULL;
}
