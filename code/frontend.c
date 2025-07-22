#include "frontend.h"
#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
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
                for(int k=1; k< GAME_HEIGHT;k++){
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

void display_bird(bird_t * bird, int ch){
    static int up=0;
    if(ch==' '){
        up=!up;
    }
    else if(bird->delta_y>0){
        mvaddch(bird->y,bird->x,'~');
    }
    else if(up){
            mvaddch(bird->y,bird->x,'v');
        }
        else{
            mvaddch(bird->y,bird->x,'^');
        }
    
}
void display_main_menu( int selection){
    const char *opciones[] = {
        "START",
        "EXIT"
    };
    clear();
    const char title[]="=== FLAPPY MAIN MENU ===";
    mvprintw(1, (GAME_WIDTH-strlen(title))/2, &(title[0]));
    for (int i = 0; i < NUM_OPTIONS_MAIN; i++) {
        if (i == selection) attron(A_REVERSE);
        mvprintw(3 + i, 7, "%s", opciones[i]);
        if (i == selection) attroff(A_REVERSE);
    }

    refresh();
}
void display_pause(int selection){
    const char *opciones[] = {
        "CONTINUE",
        "EXIT",
        "RESTART"
    };
    clear();
    mvprintw(1, 5, "=== FLAPPY PAUSE MENU ===");
    for (int i = 0; i < NUM_OPTIONS_MAIN; i++) {
        if (i == selection) attron(A_REVERSE);
        mvprintw(3 + i, 7, "%s", opciones[i]);
        if (i == selection) attroff(A_REVERSE);
    }

    refresh();
}