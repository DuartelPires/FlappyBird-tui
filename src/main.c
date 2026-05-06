#include "board.h"
#include "display.h"
#include "game.h"
#include <pthread.h>
#include <stdio.h>
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

  pthread_t drawThread;
  pthread_create(&drawThread, NULL, draw_loop, (void *)&currentSession);

  char input;
  while ((input = get_input()) != 'Q') {
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

    usleep(10000); // Small sleep to avoid hogging CPU
  }

  currentSession.gameBird.alive = 0;
  pthread_join(gameThread, NULL);
  pthread_join(drawThread, NULL);
  pthread_mutex_destroy(&gameSessionLock);

  terminal_cleanup();
  return 0;
}