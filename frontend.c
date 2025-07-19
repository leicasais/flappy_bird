#include "frontend.h"
#include "backend.h"

//Global var from main.c
extern int WIDTH;
extern int HEIGHT;
extern int HOLE_HEIGHT;
extern int NUM_COL;
extern int COL_WIDTH;
extern int SPACE;
extern float GRAVITY;
extern float JUMP_VEL;   


//Function
void display_col(column_t* pcol){
    for(int i=0; i< NUM_COL; i++){
        if(pcol[i].len >0){
            for(int j=0;j<pcol[i].len;j++){
                for(int k=1; k< HEIGHT;k++){
                    if( k == (pcol[i].y) ){
                        k+=HOLE_HEIGHT;
                    }
                    mvaddch(k,(pcol[i].x)+j,'*');
                }
            
            }  
        }   
    }
    mvprintw(0, 0, "Floppy bird"); 
}

void display_bird(bird_t * bird){
    int row = (int)(bird->y + 0.5f);
    int col = (int)(bird->x + 0.5f);
    mvaddch(row,col,'^');
}
void display_main_menu( int selection){
    const char *opciones[] = {
        "START",
        "EXIT"
    };
    clear();
    mvprintw(1, 5, "=== FLAPPY MAIN MENU ===");
    for (int i = 0; i < NUM_OPTIONS_MAIN; i++) {
        if (i == selection) attron(A_REVERSE);
        mvprintw(3 + i, 7, "%s", opciones[i]);
        if (i == selection) attroff(A_REVERSE);
    }

    refresh();
}
