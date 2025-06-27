#include "backend.h"
#include "frontend.h"
//Variables Globales

int main(void)
{
    initscr();  
    cbreak();   //Read keys not waiting for Enter 
    noecho();   
    keypad(stdscr, TRUE);
    curs_set(FALSE);        // Oculta el cursor
    timeout(20);           // `getch()` waits in ms.Calc FPS: (t[ms]*10^-3)^-1. 50 FPS
    srand(time(NULL));     //plant the seed for rand()

    //var
    coord_t column[NUM_COL];
    coord_t bird; //init bird

    //main program
    init(column, &bird);
    while (1)
    {
        //Update game logic
        col_mov(column);
        //Update display
        erase(); 
        display_col(column);
        refresh();
        usleep(333333);     //3 FPS
        
    }
    
    
    endwin(); // exits ncurses mode, returns control to the terminal
    return 0;
}