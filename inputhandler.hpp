#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <ncurses.h>
#include <string>

using namespace std;

class InputHandler {
private:
  char mode;
public:
  InputHandler(char);

  void updateMode(char);
  string handle(int);
};

#endif
