#ifndef PIPE_H
#define PIPE_H

#include "board.h"

typedef struct {
  session *s;
  int index;
} pipe_arg;

void *individualPipeLoop(void *arg);
void placePipeRight(session *currentSession, int index);

#endif
