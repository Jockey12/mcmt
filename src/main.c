#include "libs.h"

#define TB_HEIGHT 3
#define TB_WIDTH 14
#define GENERATE_COUNT 100
#define MAX_LINES 10
#define LINE_BUF 512

static void render_input(WINDOW *win, char *buf, int *buf_len, int inner_h,
                         int inner_w, int *out_row, int *out_col) {
  // werase(win);
  box(win, 0, 0);

  int row = 0, col = 0;
  for (int i = 0; i < *buf_len; i++) {
    if (row >= inner_h) {
      *buf_len = i;
    }
    if (buf[i] == '\n') {
      row++;
      col = 0;
    } else {
      mvwaddch(win, 1 + row, 1 + col, (chtype)buf[i]);
      col++;
      if (col >= inner_w) {
        row++;
        col = 0;
      }
    }
  }
  *out_col = col;
  *out_row = row;
}

void drawOuter() { box(stdscr, 0, 0); }

void stopWatch() {
  WINDOW *timer_window = newwin(3, 20, 1, 1);

  if (!timer_window) {
    endwin();
    fprintf(stderr, "err");
    exit(EXIT_FAILURE);
  }
  box(timer_window, 0, 0);
  wrefresh(timer_window);
  time_t start = time(NULL);
  nodelay(stdscr, TRUE);
  while (1) {
    time_t now = time(NULL);

    wrefresh(timer_window);

    double elaspedT = difftime(time(NULL), start);
    werase(timer_window);
    box(timer_window, 0, 0);
    mvwprintw(timer_window, 1, 1, "elapsed: %.2f s", elaspedT);
    wrefresh(timer_window);
    napms(200);
  }
}

int main() {
  // words array
  const char *words[] = {
      "this",     "is",       "a",       "text",     "execute",     "thing",
      "that",     "arch",     "types",   "the",      "words",       "typing",
      "keyboard", "terminal", "example", "fast",     "static",      "dynamic",
      "options",  "words",    "list",    "program",  "programming", "move",
      "neovim",   "linux",    "fancy",   "navigate", "forget",      "new",
      "that",     "there",    "attempt", "lazy"};
  int word_count = sizeof(words) / sizeof(words[0]);
  // int total_length = 0;
  int generated_count = 100;
  int width, height;
  int ch;
  char str[90];
  char buf[LINE_BUF];
  int buf_len = 0;
  int out_r = 0;
  int out_c = 0;
  initscr();
  noecho();
  keypad(stdscr, TRUE);

  getmaxyx(stdscr, height, width);
  srand(time(NULL));
  drawOuter();
  refresh();
  const char *word_dict[generated_count];
  for (int i = 0; i < generated_count; i++) {
    word_dict[i] = words[rand() % word_count];
  }

  // padding, inner box pos
  int pad_y = 2;
  int pad_x = 4;
  int box_h = height - pad_y * 2;
  int box_w = width - pad_x * 2;
  int box_y = pad_y;
  int box_x = pad_x;

  // inner box
  WINDOW *wordwin = newwin(box_h, box_w, box_y, box_x);
  box(wordwin, 0, 0);
  wrefresh(wordwin);

  // print out random array of words inside inner box
  int row = 1;
  int col = 1;
  for (int n = 0; n < generated_count; n++) {
    const char *w = word_dict[n];
    int len = strlen(w);
    if (col + len >= box_w - 1) {
      row++;
      col = 1;
    }
    if (row >= box_h - 1) {
      break;
    }
    mvwprintw(wordwin, row, col, "%s ", w);
    col += len + 1;
  }
  stopWatch();
  wrefresh(wordwin);

  // if user press C-c; quit
  while (1) {
    ch = getch();
    if (ch == 3) {
      break;
    } else if (ch >= 32 && ch <= 127) {
      if (buf_len < LINE_BUF) {
        buf[buf_len++] = ch;
      }
    }
    render_input(wordwin, buf, &buf_len, box_h - 2, box_w - 2, &out_r, &out_c);
    wmove(wordwin, 1 + out_r, 1 + out_c);
    wrefresh(wordwin);
  }
  // echo();
  nocbreak();
  refresh();
  getstr(str);

  endwin();
  return 0;
}
