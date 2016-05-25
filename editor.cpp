#include "editor.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

Editor::Editor() {
  x = 0;y = 0;wx = 0;wy = 0;mode = 'n';
  status = "Normal Mode";
  filename = "untitled";

  buff = new Buffer();
  buff->appendLine("");
}

Editor::Editor(string fn) {
  x = 0;y = 0;wx = 0;wy = 0;mode = 'n';
  status = "Normal Mode";
  filename = fn;

  buff = new Buffer();

  ifstream infile(fn.c_str());
  if(infile.is_open()) {
    while(!infile.eof()) {
      string temp;
      getline(infile, temp);
      buff->appendLine(temp);
    }
  }
  else {
    cerr << "Cannot open file: '" << fn << "'\n";
    buff->appendLine("");
  }
}

void Editor::updateStatus() {
  switch(mode) {
  case 'n':
    status = "Normal Mode";
    break;
  case 'i':
    status = "Insert Mode";
    break;
  case 'x':
    status = "Exiting...";
    break;
  }
  status += "\tCOL: " + to_string(wx+x+1) + "\tROW: " + to_string(wy+y+1) + "\tDEBUG: " + to_string(wx) + " " + to_string(x);
}

void Editor::handleInput(int c) {
  switch(c) {
  case KEY_LEFT:
    moveLeft();
    return;
  case KEY_RIGHT:
    moveRight();
    return;
  case KEY_UP:
    moveUp();
    return;
  case KEY_DOWN:
    moveDown();
    return;
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

void Editor::moveLeft() {
  if(wx+x-1 >= 0) {
    x--;
    move(y,x);
  }
  if(x == 0 && wx != 0) {
    scrollLeft();
  }
}

void Editor::moveRight() {
  if(wx+x+1 <= (int)buff->lines[wy+y].length()) {
    if(x+1 > COLS) {
      scrollRight();
    }
    else {
      x++;
    }
    move(y,x);
  }
}

void Editor::scrollLeft() {
  wx--;
}

void Editor::scrollRight() {
  wx++;
}

void Editor::moveUp() {
  if(y-1 >= 0) {
    y--;
  }
  if(y == 0 && wy != 0) {
    scrollUp();
  }
  if(wx+x >= (int)buff->lines[y+wy].length()) {
    x = buff->lines[y+wy].length() - wx;
    while(x < 0)scrollRight();
  }
  move(y, x);
}

void Editor::moveDown() {
  if(wy+y+1 < (int)buff->lines.size()) {
    if(y+1 > LINES-2) {
      scrollDown();
    }
    else {
      y++;
    }
  }
  if(wx+x >= (int)buff->lines[y+wy].length()) {
    x = buff->lines[y+wy].length() - wx;
    while(x < 0)scrollRight();
  }
  move(y, x);
}

void Editor::scrollUp() {
  wy--;
}

void Editor::scrollDown() {
  wy++;
}


void Editor::printBuff() {
  for(int i = wy; i<LINES-1+wy; ++i) {
    if(i >= (int)buff->lines.size()) {
      move(i, 0);
      clrtoeol();
    }
    else {
      mvprintw(i-wy, 0, (buff->lines[i]).substr(wx, min((int)buff->lines[i].length(), COLS)).c_str());
    }
    clrtoeol();
  }
  move(y, x);
}

void Editor::scrollToCorrect() {
  while(x < 0) {scrollLeft();}
  while(x > COLS) {scrollRight();}
  while(y < 0) {scrollUp();}
  while(y > LINES) {scrollDown();}
}

void Editor::printStatusLine() {
  attron(A_REVERSE);
  mvprintw(LINES-1, 0, status.c_str());
  clrtoeol();
  attroff(A_REVERSE);
}

void Editor::deleteLine() {
  buff->removeLine(y);
}

void Editor::deleteLine(int i) {
  buff->removeLine(i);
}

void Editor::saveFile() {
  if(filename == "") {
    filename = "untitled";
  }
  ofstream f(filename.c_str());
  if(f.is_open()) {
    for(int i = 0; i<(int)buff->lines.size(); ++i) {
      f << buff->lines[i] << endl;
    }
    status = "Saved to file!";
  }
  else {
    status = "Error: Cannot open file for writing!";
  }
  f.close();
}

