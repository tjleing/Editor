#include <ncurses.h>
#include "editor.hpp"

#define WORLD_WIDTH 50
#define WORLD_HEIGHT 20

void ncurses_setup() {
    initscr();
    noecho();
    raw();
    keypad(stdscr, TRUE);
}

int main(int argc, char* argv[]) {
    Editor ed;
    string fn = "";
    if(argc > 1) {
         fn = string(argv[1]);
         ed = Editor(fn);
    }
    else {
         ed = Editor();
    }
    ncurses_setup();
    while(ed.getMode() != 'x') {
         ed.updateStatus();
         ed.printStatusLine();
         ed.printBuff();
         int input = getch();
         ed.handleInput(input);
    }
    refresh();
    endwin();
    return 0;
}
