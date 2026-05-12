#include "pipe.h"
#include "board.h"
#include "game.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

extern pthread_mutex_t gameSessionLock;

void placePipe(session *currentSession, int index) {
  currentSession->pipes[index].x = BOARD_WIDTH - 1;
  if (!currentSession->pipes[index].isVisible) {
    currentSession->pipes[index].y = (rand() % (BOARD_HEIGHT - 10)) + 7;
    currentSession->pipes[index].topY = currentSession->pipes[index].y - 6;
    currentSession->pipes[index].isVisible = 1;
  }
  setStartingBoard(currentSession);
}

int checkPipeReachedCorner(session *currentSession, int index) {
  if (currentSession->pipes[index].x + currentSession->pipes[index].width <=
      0) {
    return 1;
  }
  return 0;
}

int checkCollisionWithBird(session *currentSession, int index) {
  if (!currentSession->pipes[index].isVisible) {
    return 0;
  }

  int x = currentSession->pipes[index].x;
  int y = currentSession->pipes[index].y;
  int topY = currentSession->pipes[index].topY;
  int width = currentSession->pipes[index].width;

  int birdX = currentSession->gameBird.x;
  int birdY = currentSession->gameBird.y;
  int birdWidth = currentSession->gameBird.width;
  int birdHeight = currentSession->gameBird.height;

  if (birdX + birdWidth <= x || birdX >= x + width) {
    return 0;
  }

  if (birdY < topY || birdY + birdHeight > y) {
    return 1;
  }

  return 0;
}

void tryKillBird(session *currentSession, int index) {
  if (!checkCollisionWithBird(currentSession, index)) {
    return;
  }
  currentSession->gameBird.alive = 0;
}

void movePipe(session *currentSession, int index) {
  currentSession->pipes[index].x--;

  // if pipe reached the corner resets position
  if (checkPipeReachedCorner(currentSession, index)) {
    currentSession->pipes[index].isVisible = 0;
    placePipe(currentSession, index);
  } else {
    setStartingBoard(currentSession);
  }
}

void *individualPipeLoop(void *arg) {
  pipe_arg *p_arg = (pipe_arg *)arg;
  session *currentSession = p_arg->s;
  int index = p_arg->index;

  waitTime(currentSession->pipes[index].creationDelay);

  pthread_mutex_lock(&gameSessionLock);
  placePipe(currentSession, index);
  pthread_mutex_unlock(&gameSessionLock);

  while (currentSession->gameBird.alive) {
    pthread_mutex_lock(&gameSessionLock);
    movePipe(currentSession, index);
    tryKillBird(currentSession, index);
    checkForPoints(currentSession, index);
    pthread_mutex_unlock(&gameSessionLock);

    waitTime(100);
  }

  free(p_arg);
  return NULL;
}
