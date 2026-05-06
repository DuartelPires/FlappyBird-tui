#include "board.h"
#include "main.h"
#include <pthread.h>
#include <stdio.h>

void resetBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      board[i][j] = ' ';
    }
  }
}

void initSession(session *s) {
  s->score = 0;
  s->gameBird.x = BOARD_WIDTH / 2 - 1;
  s->gameBird.y = BOARD_HEIGHT / 2 - 1;
  s->gameBird.alive = 1;
  s->gameBird.width = 2;
  s->gameBird.height = 2;
  s->gameBird.birdColor = 1;
  s->gameBird.birdSymbol = '#';
  resetBoard(s->gameBoard);
}

void putOnBoard(session *currentSession, int x, int y, char c, int width,
                int height) {
  for (int i = y; i < y + height && i < BOARD_HEIGHT; i++) {
    for (int j = x; j < x + width && j < BOARD_WIDTH; j++) {
      currentSession->gameBoard[i][j] = c;
    }
  }
}

void setBirdPosition(session *currentSession, int x, int y) {
  currentSession->gameBird.x = x;
  currentSession->gameBird.y = y;
}

void setStartingBoard(session *currentSession) {
  resetBoard(currentSession->gameBoard);
  putOnBoard(currentSession, currentSession->gameBird.x,
             currentSession->gameBird.y, currentSession->gameBird.birdSymbol,
             currentSession->gameBird.width, currentSession->gameBird.height);
}
