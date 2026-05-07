#ifndef BOARD_H
#define BOARD_H

#define BOARD_HEIGHT 26
#define BOARD_WIDTH 80

#include <pthread.h>

typedef struct {
  int x;
  int y;
  int alive;
  int width;
  int height;
  int color;
  char symbol;
} bird;

typedef struct {
  int x;
  int y;
  int width;
  int height;
  int isDown;
  int color;
  char symbol;
  int isVisible;
  pthread_t pipeThread;
  int creationDelay;
} pipeWall;

typedef struct {
  int score;
  char gameBoard[BOARD_HEIGHT][BOARD_WIDTH];
  bird gameBird;
  pipeWall *pipes;
  int pipeCount;
} session;

void resetBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]);
void initSession(session *s);
void putOnBoard(session *currentSession, int x, int y, char c, int width,
                int height);
void setBirdPosition(session *currentSession, int x, int y);
void setStartingBoard(session *currentSession);

#endif
