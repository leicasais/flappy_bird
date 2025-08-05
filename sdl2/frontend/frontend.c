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

void display_bird(bird_t *bird, app_t *app){
    int fly_frames[] = {0, 6, 6, 7};
    //&srcRect the position and size in pixeles that you want to cut off the img
    SDL_Rect scr_rect;
    int frame_index = fly_frames[bird->current_frame];
    scr_rect.x = (frame_index % 4) * HITBOX_X;          //gives the index of the column of the bird matrix
    scr_rect.y = (frame_index / 4) * HITBOX_Y;
    scr_rect.w = HITBOX_X;
    scr_rect.h = HITBOX_Y;

    // &destRect sets the size of the cut img and where to plaice it in the screen
    SDL_Rect dest_rect = {.x=bird->x_top, .y=bird->y_top, .w= (bird->x_bottom - bird->x_top), .h= bird->y_bottom - bird->y_top} ; 
    SDL_RenderCopy(app->renderer, bird->texture, &scr_rect, &dest_rect);       // Copies a part of a texture (src_rect) and draws it in the screen using the active render



}

/*Function
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
}
void display_upper_line(menu_t menu) {
    mvprintw(0, 0, "Lives: ");
    for (int i = 0; i < menu.lives; i++) {
        printw(" <3 "); // harts
    }
    char str[20];
    sprintf(str, "Score: %d", menu.score);  // Convierte el número en texto
    mvprintw(0, GAME_WIDTH-strlen(str)-1, "%s", str);
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

void display_resurecting_bird(bird_t * bird, int ch){
    static int up=0;
    static int time=0;
    if(ch==' '){
        up=!up;
    }
    if(time<10){
        if(up){
            mvaddch(bird->y,bird->x,'o');
        }
        else{
            mvaddch(bird->y,bird->x,'-');
        }
    }
    if(time<20){
        time++;
    }
    else{
        time=0;
    }  
    
}*/