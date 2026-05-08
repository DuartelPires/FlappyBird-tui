#include "display.h"
#include "board.h"
#include <ctype.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

extern pthread_mutex_t gameSessionLock;

int terminal_init() {
  // Initialize ncurses mode
  initscr();

  // Disable line buffering - get characters immediately
  cbreak();

  // Don't echo typed characters to the screen
  noecho();

  // Enable special keys (arrow keys, function keys, etc.)
  keypad(stdscr, TRUE);

  // Make getch() non-blocking (return ERR if no input)
  nodelay(stdscr,
          TRUE); // Enable non-blocking input for independent ghost movement

  // Hide the cursor
  curs_set(0);

  // Enable color if terminal supports it
  if (has_colors()) {
    start_color();

    // Define color pairs (foreground, background)
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // Pacman
    init_pair(2, COLOR_RED, COLOR_BLACK);     // Ghosts
    init_pair(3, COLOR_BLUE, COLOR_BLACK);    // Walls
    init_pair(4, COLOR_WHITE, COLOR_BLACK);   // Points/dots
    init_pair(5, COLOR_GREEN, COLOR_BLACK);   // UI elements
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); // Extra
    init_pair(7, COLOR_CYAN, COLOR_BLACK);    // Extra
  }

  // Clear the screen
  clear();

  return 0;
}

void refresh_screen() {
  // Update the physical screen with the virtual screen
  refresh();
}

char get_input() {
  // Get a character from the keyboard
  int ch = getch();

  // getch() returns ERR if no input is available
  if (ch == ERR) {
    return '\0'; // No input
  }

  ch = toupper((char)ch);

  switch ((char)ch) {
  case ' ': // SPACE
  case 'Q': // quit
    return (char)ch;

  default:
    return '\0';
  }
}

void terminal_cleanup() {
  // Restore terminal settings and clean up ncurses
  endwin();
}
void draw_board(session *s) {
  // Erase the screen (more gentle than clear() to avoid flickering)
  erase();

  // Draw board content
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      char c = s->gameBoard[i][j];
      if (c != ' ') {
        if (c == s->gameBird.symbol) {
          attron(COLOR_PAIR(1));
          mvaddch(i + 1, j + 1, c);
          attroff(COLOR_PAIR(1));
        } else if (c == s->pipes[0].symbol) {
          attron(A_REVERSE); // Inverte a cor (o fundo passa a ser a cor do texto)
          mvaddch(i+1, j+1, ' '); // Desenha um espaço (que parecerá um bloco sólido)
          attroff(A_REVERSE);
          //attron(COLOR_PAIR(2));
          //mvaddch(i + 1, j + 1, c);
          //attroff(COLOR_PAIR(2));
        } else {
          mvaddch(i + 1, j + 1, c);
        }
      }
    }
  }

  attron(COLOR_PAIR(3));
  for (int i = 0; i <= BOARD_HEIGHT + 1; i++) {
    mvaddch(i, 0, ACS_VLINE);
    mvaddch(i, BOARD_WIDTH + 1, ACS_VLINE);
  }
  for (int j = 0; j <= BOARD_WIDTH + 1; j++) {
    mvaddch(0, j, ACS_HLINE);
    mvaddch(BOARD_HEIGHT + 1, j, ACS_HLINE);
  }
  // add corners
  mvaddch(0, 0, ACS_ULCORNER);
  mvaddch(0, BOARD_WIDTH + 1, ACS_URCORNER);
  mvaddch(BOARD_HEIGHT + 1, 0, ACS_LLCORNER);
  mvaddch(BOARD_HEIGHT + 1, BOARD_WIDTH + 1, ACS_LRCORNER);
  attroff(COLOR_PAIR(3));

  // Draw score
  attron(COLOR_PAIR(5));
  mvprintw(BOARD_HEIGHT + 2, 0, "Score: %d", s->score);
  attroff(COLOR_PAIR(5));

  refresh_screen();
}

void *draw_loop(void *arg) {
  session *s = (session *)arg;
  while (s->gameBird.alive) {
    pthread_mutex_lock(&gameSessionLock);
    draw_board(s);
    pthread_mutex_unlock(&gameSessionLock);

    usleep(33333); // ~30 FPS
  }
  return NULL;
}
