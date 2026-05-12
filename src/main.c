#include "board.h"
#include "display.h"
#include "game.h"
#include "pipe.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t gameSessionLock;

int checkInput(session *currentSession) {
  char input;
  pthread_mutex_lock(&gameSessionLock);
  input = get_input();
  pthread_mutex_unlock(&gameSessionLock);

  if (input == 'Q')
    return 1;

  if (input == ' ') {
    pthread_mutex_lock(&gameSessionLock);
    birdJump(currentSession);
    pthread_mutex_unlock(&gameSessionLock);
  }

  usleep(10000);
  return 0;
}

void initiatePipeThreads(session *currentSession) {
  for (int i = 0; i < currentSession->pipeCount; i++) {
    pipe_arg *arg = malloc(sizeof(pipe_arg));
    arg->s = currentSession;
    arg->index = i;
    pthread_create(&currentSession->pipes[i].pipeThread, NULL,
                   individualPipeLoop, arg);
  }
}

int main() {
  session currentSession;

  pthread_mutex_init(&gameSessionLock, NULL);

  initSession(&currentSession);
  terminal_init();

  setStartingBoard(&currentSession);
  draw_board(&currentSession);

  // initiate game thread
  pthread_t gameThread;
  pthread_create(&gameThread, NULL, game_loop, (void *)&currentSession);

  // initiate pipe thread
  initiatePipeThreads(&currentSession);

  // initiate draw thread
  pthread_t drawThread;
  pthread_create(&drawThread, NULL, draw_loop, (void *)&currentSession);

  // check user input
  while (1) {
    if (checkInput(&currentSession))
      break;
  }

  currentSession.gameBird.alive = 0;
  pthread_join(gameThread, NULL);
  pthread_join(drawThread, NULL);
  pthread_mutex_destroy(&gameSessionLock);

  terminal_cleanup();
  return 0;
}
