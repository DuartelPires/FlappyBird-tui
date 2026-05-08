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
  if (currentSession->pipes[index].x + currentSession->pipes[index].width 
    <= 0) {
    return 1;
  }
  return 0;
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

void *individual_pipe_loop(void *arg) {
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
    checkForPoints(currentSession, index);
    pthread_mutex_unlock(&gameSessionLock);

    waitTime(100);
  }

  free(p_arg);
  return NULL;
}
