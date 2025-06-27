#include "frontend.h"
#include "backend.h"

//Global var from main.c
extern int WIDTH;
extern int HEIGHT;
extern int HOLE_HEIGHT;
extern int NUM_COL;
extern int COL_WIDTH;
extern int SPACE;


//Function
void display_col(coord_t* pcol){
    for(int i=0; i< NUM_COL; i++){
        for(int j=0;j<lines_col(pcol[i]);j++){
            for(int k=1; k< HEIGHT;k++){
                if( k == (pcol[i].y) ){
                    k+=HOLE_HEIGHT;
                }
                mvaddch(k,(pcol[i].x)+j,'*');
            }
            
        }  
        mvprintw(0, 0, "Floppy bird"); 
    }
}
