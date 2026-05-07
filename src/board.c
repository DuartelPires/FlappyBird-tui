#include "board.h"
#include "main.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void resetBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      board[i][j] = ' ';
    }
  }
}

void initSession(session *s) {

  // initialize bird
  s->score = 0;
  s->gameBird.x = BOARD_WIDTH / 2 - 1;
  s->gameBird.y = BOARD_HEIGHT / 2 - 1;
  s->gameBird.alive = 1;
  s->gameBird.width = 2;
  s->gameBird.height = 2;
  s->gameBird.color = 1;
  s->gameBird.symbol = '#';

  s->pipeCount = 3;
  s->pipes = (pipeWall *)malloc(s->pipeCount * sizeof(pipeWall));

  int delay = 0;

  // initialize pipes
  for (int i = 0; i < s->pipeCount; i++) {
    s->pipes[i].x = -1;
    s->pipes[i].y = -1;
    s->pipes[i].width = 3;
    s->pipes[i].height = 100;
    s->pipes[i].isDown = 1;
    s->pipes[i].color = 2;
    s->pipes[i].symbol = '@';
    s->pipes[i].isVisible = 0;
    s->pipes[i].creationDelay = delay;
    delay += 3000;
  }

  resetBoard(s->gameBoard);
}

void putOnBoard(session *currentSession, int x, int y, char c, int width,
                int height) {
  for (int i = y; i < y + height && i < BOARD_HEIGHT; i++) {
    for (int j = x; j < x + width && j < BOARD_WIDTH; j++) {
      if (i >= 0 && j >= 0) {
        currentSession->gameBoard[i][j] = c;
      }
    }
  }
}

void setBirdPosition(session *currentSession, int x, int y) {
  currentSession->gameBird.x = x;
  currentSession->gameBird.y = y;
}

void setStartingBoard(session *currentSession) {
  resetBoard(currentSession->gameBoard);

  // Draw bird
  putOnBoard(currentSession, currentSession->gameBird.x,
             currentSession->gameBird.y, currentSession->gameBird.symbol,
             currentSession->gameBird.width, currentSession->gameBird.height);

  // Draw pipes
  for (int i = 0; i < currentSession->pipeCount; i++) {
    if (currentSession->pipes[i].isVisible) {
      putOnBoard(currentSession, currentSession->pipes[i].x,
                 currentSession->pipes[i].y, currentSession->pipes[i].symbol,
                 currentSession->pipes[i].width,
                 currentSession->pipes[i].height);
    }
  }
}
