#include "libs.h"
#include <ncurses.h>
#define TB_HEIGHT 3
#define TB_WIDTH 14

void drawTitleBox() {
  int width, height;
  getmaxyx(stdscr, height, width);
  newwin(TB_HEIGHT, TB_WIDTH, height / 2, width / 2);
}

void drawOuter() { box(stdscr, 0, 0); }
int main() {
  const char *words[] = {
      "this",     "is",       "a",       "text",     "execute",     "thing",
      "that",     "arch",     "types",   "the",      "words",       "typing",
      "keyboard", "terminal", "example", "fast",     "static",      "dynamic",
      "options",  "words",    "list",    "program",  "programming", "move",
      "neovim",   "linux",    "fancy",   "navigate", "forget",      "new",
      "that",     "there",    "attempt", "lazy"};

  initscr();
  int width, height;
  getmaxyx(stdscr, height, width);
  cbreak();
  noecho();
  // nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  noqiflush();
  srand(time(NULL));
  drawOuter();
  mvprintw(height - 2, 1, "press q to quit");
  refresh();

  int word_count = sizeof(words) / sizeof(words[0]);
  int total_length = 0;
  int generated_count = 20;
  const char *word_dict[generated_count];
  for (int i = 0; i < generated_count; i++) {
    word_dict[i] = words[rand() % word_count];
  }
  // word_dict[generated_count] = words[rand() % word_count];

  move(6, 10);
  int cWord = 0;
  for (int n = 0; n < generated_count; n++) {
    // printw("%s ", words[n]);
    mvprintw(height / 2, width / 2, "%s ", word_dict[n]);
    cWord++;
    word_dict[cWord] = words[rand() % word_count];
  }
  refresh();

  int ch;
  while ((ch = getch()) != 'q') {
    mvprintw(height, 1, "pressed: %d  ", ch);
    // refresh();
  }
  // drawBox();

  endwin();
  return 0;
}
