#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>  

#define WIDTH 40       // num colum
#define HEIGHT 20      // num lines
#define COL_WIDTH 3     //width of columns 
#define NUM_COL WIDTH/(COL_WIDTH) ////Num of columns, 3 spaces between col

typedef struct{     
    int x[COL_WIDTH]; 
    int y[HEIGHT];      //Saves where are the holes

}column_t;

int main(void)
{
    initscr();  
    cbreak();   //Read keys not waiting for Enter 
    noecho();   
    keypad(stdscr, TRUE);
    curs_set(FALSE);        // Oculta el cursor
    timeout(20);           // `getch()` waits in ms.Calc FPS: (t[ms]*10^-3)^-1. 50 FPS

    //var
    column_t column[WIDTH/6]; 

    endwin(); // exits ncurses mode, returns control to the terminal
    return 0;
}

//Funciones 

void init(column_t* pcol){
    srand(time(NULL));
    for(int i=0; i<NUM_COL;i++){
        pcol[i].x=
    }
    
}