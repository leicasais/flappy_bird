#include "backend.h"

int main(void)
{
    initscr();  
    cbreak();   //Read keys not waiting for Enter 
    noecho();   
    keypad(stdscr, TRUE);
    curs_set(FALSE);        // Oculta el cursor
    timeout(20);           // `getch()` waits in ms.Calc FPS: (t[ms]*10^-3)^-1. 50 FPS

    //var
    coord_t column[NUM_COL];
    coord_t bird; //init bird

    endwin(); // exits ncurses mode, returns control to the terminal
    return 0;
}