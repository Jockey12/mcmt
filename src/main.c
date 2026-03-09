#include "libs.h"
#include <ncurses.h>

#define TB_HEIGHT 3
#define TB_WIDTH 14
#define GENERATE_COUNT 100
#define MAX_LINES 10
#define LINE_BUF 1024
#define TARGET_BUF 2048

#define COLOR_CORRECT 1
#define COLOR_WRONG 2

// Renders target text with correct/wrong coloring and a block cursor at
// buf_len.
static void render_words(WINDOW *win, const char *target, int target_len,
                         const char *buf, int buf_len, int inner_h,
                         int inner_w) {
  werase(win);
  box(win, 0, 0);

  int row = 0, col = 0;
  for (int i = 0; i < target_len; i++) {
    if (row >= inner_h)
      break;

    if (i < buf_len) {
      if (buf[i] == target[i]) {
        wattron(win, COLOR_PAIR(COLOR_CORRECT));
        mvwaddch(win, 1 + row, 1 + col, target[i]);
        wattroff(win, COLOR_PAIR(COLOR_CORRECT));
      } else {
        wattron(win, COLOR_PAIR(COLOR_WRONG));
        mvwaddch(win, 1 + row, 1 + col, target[i]);
        wattroff(win, COLOR_PAIR(COLOR_WRONG));
      }
    } else if (i == buf_len) {
      // block cursor: highlight the next letter to type
      wattron(win, A_REVERSE);
      mvwaddch(win, 1 + row, 1 + col, target[i]);
      wattroff(win, A_REVERSE);
    } else {
      mvwaddch(win, 1 + row, 1 + col, target[i]);
    }

    col++;
    if (col >= inner_w) {
      row++;
      col = 0;
    }
  }
}

void drawOuter() { box(stdscr, 0, 0); }

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
  int generated_count = 150;
  int width, height;
  int ch;
  char buf[LINE_BUF];
  int buf_len = 0;
  char target[TARGET_BUF];
  int target_len = 0;
  int typing_started = 0;
  time_t start_time = 0;
  initscr();
  start_color();
  use_default_colors();
  init_pair(COLOR_CORRECT, COLOR_GREEN, -1);
  init_pair(COLOR_WRONG, COLOR_RED, -1);
  if (!has_colors()) {
    endwin();
    printf("No color support\n");
    return 1;
  }
  init_pair(COLOR_CORRECT, COLOR_GREEN, COLOR_BLACK);
  init_pair(COLOR_WRONG, COLOR_RED, COLOR_BLACK);
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  getmaxyx(stdscr, height, width);
  srand(time(NULL));
  drawOuter();
  refresh();
  const char *word_dict[generated_count];
  for (int i = 0; i < generated_count; i++) {
    word_dict[i] = words[rand() % word_count];
  }

  // build flat target string from word_dict
  for (int i = 0; i < generated_count; i++) {
    int wlen = strlen(word_dict[i]);
    if (target_len + wlen + 1 >= TARGET_BUF)
      break;
    memcpy(target + target_len, word_dict[i], wlen);
    target_len += wlen;
    if (i < generated_count - 1)
      target[target_len++] = ' ';
  }
  target[target_len] = '\0';

  // padding, inner box pos
  int pad_y = 2;
  int pad_x = 4;
  int box_h = height - pad_y * 2;
  int box_w = width - pad_x * 2;
  int box_y = pad_y;
  int box_x = pad_x;

  // inner box
  WINDOW *wordwin = newwin(box_h, box_w, box_y, box_x);
  int inner_h = box_h - 2;
  int inner_w = box_w - 2;

  render_words(wordwin, target, target_len, buf, buf_len, inner_h, inner_w);
  wrefresh(wordwin);

  // timer window - bottom right borderless
  int tw_w = 12;
  WINDOW *timer_win = newwin(1, tw_w, height - 2, width - tw_w - 2);
  wrefresh(timer_win);
  //

  // if user press C-c; quit
  while (1) {
    ch = getch();
    if (ch == 3) {
      break;
    } else if (ch != ERR) {
      if (!typing_started && ch >= 32 && ch <= 126) {
        typing_started = 1;
        start_time = time(NULL);
      }
      if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
        if (buf_len > 0)
          buf_len--;
      } else if (ch == '\n' || ch == KEY_ENTER) {
        // treat Enter as a space
        if (buf_len < LINE_BUF - 1)
          buf[buf_len++] = ' ';
      } else if (ch >= 32 && ch <= 126) {
        if (buf_len < LINE_BUF - 1)
          buf[buf_len++] = ch;
      }
      render_words(wordwin, target, target_len, buf, buf_len, inner_h, inner_w);
      wrefresh(wordwin);
    }
    if (typing_started) {
      double elapsed = difftime(time(NULL), start_time);
      werase(timer_win);
      mvwprintw(timer_win, 0, 0, "%ds", (int)elapsed);
      wrefresh(timer_win);
    }
    if (ch == ERR)
      napms(100);
  }
  nocbreak();
  endwin();
  return 0;
}
