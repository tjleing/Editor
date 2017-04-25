#include "inputhandler.hpp"

InputHandler::InputHandler(char md) {
  mode = md;
}

string InputHandler::handle(int c) {
  switch(c) {
  case KEY_LEFT:
    return "moveLeft";
  case KEY_RIGHT:
    return "moveRight";
  case KEY_UP:
    return "moveUp";
  case KEY_DOWN:
    return "moveDown";
  }
  switch(mode) {
  case 'n':
    switch(c) {
    case 'x':
      mode = 'x';
      break;
    case 'i':
      mode = 'i';
      break;
    case 's':
      saveFile();
      break;
    case 'h':
      moveLeft();
      return;
    case 'l':
      moveRight();
      return;
    case 'k':
      moveUp();
      return;
    case 'j':
      moveDown();
      return;
    }
    break;
  case 'i':
    switch(c) {
    case 27:
      // ESC
      mode = 'n';
      break;
    case 127:
    case KEY_BACKSPACE:
      // Backspace
      if(x+wx == 0 && y+wy > 0) {
        x = buff->lines[y+wy-1].length();
        // wx guaranteed to be 0
        // bring line down
        buff->lines[y+wy-1] += buff->lines[y+wy];
        // delete current line
        deleteLine();
        moveUp();
      }
      else {
        // remove a character
        buff->lines[y+wy].erase(--x+wx, 1);
      }
      scrollToCorrect();
      break;
    case KEY_DC:
      // Delete
      if(x+wx == (int)buff->lines[y+wy].length() && y+wy != (int)buff->lines.size() - 1) {
        // bring line down
        buff->lines[y+wy] += buff->lines[y+wy+1];
        // delete the line
        deleteLine(y+wy+1);
      }
      else {
        buff->lines[y+wy].erase(x+wx, 1);
      }
      break;
    case KEY_ENTER:
    case 10:
      // The enter key
      // bring the rest of the line down
      if(x+wx < (int)buff->lines[y+wy].length()) {
        // rest of the line on a new line
        buff->insertLine(buff->lines[y+wy].substr(x+wx, buff->lines[y+wy].length() - x-wx), y+wy + 1);
        // remove that part of the line
        buff->lines[y+wy].erase(x+wx, buff->lines[y+wx].length() - x-wx);
      }
      else {
        buff->insertLine("", y+wy+1);
      }
      x = 0;
      moveDown();
      break;
    case KEY_BTAB:
    case KEY_CTAB:
    case KEY_STAB:
    case KEY_CATAB:
    case 9:
      // Tab key
      buff->lines[y+wy].insert(x, 4, ' ');
      x+=4;
      break;

    default:
      // any other character
      buff->lines[y+wy].insert(x+wx, 1, char(c));
      x++;
      scrollToCorrect();
      break;
    }
    break;
  }
}

