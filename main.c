#include <ncurses.h>
#include <panel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int main() {
  initscr();
  cbreak();
  noecho();
  // nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  noqiflush();
  srand(time(NULL));
  printw("hello: q to quit");
  refresh();

  const char *words[] = {
      "this",     "is",       "a",       "text",    "execute",    "thing",
      "that",     "arch",     "types",   "the",     "words",      "typing",
      "keyboard", "terminal", "example", "fast",    "static",     "dynamic",
      "options",  "words",    "list",    "program", "programming"};
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
    printw("%s ", word_dict[n]);
    cWord++;
    word_dict[cWord] = words[rand() % word_count];
  }
  refresh();

  int ch;
  while ((ch = getch()) != 'q') {
    move(1, 0);
    printw("pressed: %d  ", ch);
    refresh();
  }

  endwin();
  return 0;
}
