#include "backend.h"
#include "frontend.h"
//Variables Globales
int WIDTH;
int HEIGHT;
int HOLE_HEIGHT;
int NUM_COL;
int COL_WIDTH;
int SPACE;
float GRAVITY;   
float JUMP_VEL;   

column_t* column;

int main(void)
{
    initscr();  
    cbreak();   //Read keys not waiting for Enter 
    noecho();   
    keypad(stdscr, TRUE);
    curs_set(FALSE);        // Oculta el cursor
    timeout(20);           // `getch()` waits in ms.Calc FPS: (t[ms]*10^-3)^-1. 50 FPS
    srand(time(NULL));     //plant the seed for rand()

    //signal(SIGWINCH, handle_winch);
    getmaxyx(stdscr, HEIGHT, WIDTH);  // first read of the scren dimations (dinamic)  
    set_parameters();

    //var
    column = malloc(sizeof(column_t) * NUM_COL);
    bird_t bird; //init bird

    //main program
    init(column,&bird); 
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
    
    free(column);
    endwin(); // exits ncurses mode, returns control to the terminal
    return 0;
}