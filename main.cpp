#include <ncurses.h>

#define WORLD_WIDTH 50
#define WORLD_HEIGHT 20

int main(int argc, char* argv[]) {
    WINDOW *win;
    int offsetx, offsety;
    initscr();
    raw();
    keypad(stdscr, TRUE);
    refresh();
    offsetx = (COLS - WORLD_WIDTH) / 2;
    offsety = (LINES - WORLD_HEIGHT) / 2;
    win = newwin(WORLD_HEIGHT, WORLD_WIDTH, offsety, offsetx);
    box(win, 0, 0);
    wrefresh(win);
    printw("Hello worlds");
    wrefresh(win);
    getch();
    delwin(win);
    endwin();
    return 0;
}
