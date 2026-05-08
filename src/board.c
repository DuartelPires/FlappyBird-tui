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
    s->pipes[i].topY = -1;
    s->pipes[i].width = 3;
    s->pipes[i].height = 100;
    s->pipes[i].isDown = 1;
    s->pipes[i].color = 2;
    s->pipes[i].symbol = '@';
    s->pipes[i].isVisible = 0;
    s->pipes[i].creationDelay = delay;
    delay += 2666;
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

// ver se esta no x do pipe e se esta entre topY e y
void checkForPoints(session *currentSession, int index) {
  int birdY = currentSession->gameBird.y;
  int birdX = currentSession->gameBird.x;

  int x = currentSession->pipes[index].x;
  int y = currentSession->pipes[index].y;
  int topY = currentSession->pipes[index].topY;

  if (x == birdX && (birdY > topY && birdY < y)) {
    currentSession->score++;
  }
}

void putPipesOnBoard(session *currentSession, pipeWall pipe) {
  // top pipe
  putOnBoard(currentSession, pipe.x, 0, pipe.symbol, pipe.width, pipe.topY);
  // bottom pipe
  putOnBoard(currentSession, pipe.x, pipe.y, pipe.symbol, pipe.width,
             pipe.height);
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
      putPipesOnBoard(currentSession, currentSession->pipes[i]);
    }
  }
}
