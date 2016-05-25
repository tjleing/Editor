#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>

#include "buffer.hpp"

class Editor {
private:
  int wx, wy, x, y; // current window position in the file and on screen
  char mode;
  Buffer* buff;
  string status, filename;

  void moveUp();
  void moveDown();
  void moveLeft();
  void moveRight();

  void scrollUp();
  void scrollDown();
  void scrollLeft();
  void scrollRight();
  void scrollToCorrect();

  void deleteLine();
  void deleteLine(int);

  void saveFile();
public:
  Editor();
  Editor(string);

  char getMode() {return mode;}

  void handleInput(int);
  void printBuff();
  void printStatusLine();
  void updateStatus();
};

#endif


