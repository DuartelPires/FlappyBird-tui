#include "board.h"
#include "display.h"
#include "game.h"
#include "pipe.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t gameSessionLock;

int main() {
  session currentSession;

  pthread_mutex_init(&gameSessionLock, NULL);

  initSession(&currentSession);
  terminal_init();

  setStartingBoard(&currentSession);
  draw_board(&currentSession);

  pthread_t gameThread;
  pthread_create(&gameThread, NULL, game_loop, (void *)&currentSession);

  // initiate pipes
  for (int i = 0; i < currentSession.pipeCount; i++) {
    pipe_arg *arg = malloc(sizeof(pipe_arg));
    arg->s = &currentSession;
    arg->index = i;
    pthread_create(&currentSession.pipes[i].pipeThread, NULL,
                   individual_pipe_loop, arg);
  }

  pthread_t drawThread;
  pthread_create(&drawThread, NULL, draw_loop, (void *)&currentSession);

  // check user input
  char input;
  while (1) {
    pthread_mutex_lock(&gameSessionLock);
    input = get_input();
    pthread_mutex_unlock(&gameSessionLock);

    if (input == 'Q')
      break;

    if (input == ' ') {
      pthread_mutex_lock(&gameSessionLock);
      if (currentSession.gameBird.y > 0) {
        currentSession.gameBird.y -= 3; // Jump!
        if (currentSession.gameBird.y < 0)
          currentSession.gameBird.y = 0;
        setStartingBoard(&currentSession);
      }
      pthread_mutex_unlock(&gameSessionLock);
    }

    usleep(10000);
  }

  currentSession.gameBird.alive = 0;
  pthread_join(gameThread, NULL);
  pthread_join(drawThread, NULL);
  pthread_mutex_destroy(&gameSessionLock);

  terminal_cleanup();
  return 0;
}