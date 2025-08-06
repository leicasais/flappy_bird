#include "frontend.h"
#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int HOLE_HEIGHT;
extern int COL_BOTTOM_WIDTH;
extern int COL_TOP_HIGH;            
extern int NUM_COL;
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;
extern int BIRD_SCALE;

//windows
void prepareScene(app_t *app)   //Prepares a clean background for drawing the next frame.
{
    SDL_SetRenderDrawColor(app->renderer,185, 207, 238, 255);   //Sets the color that SDL will use when clearing or drawing, SDL_SetRenderDrawColor(app.renderer, R, G, B, A); A: opacidad (255 ultra opaco)
    SDL_RenderClear(app->renderer);         //Clears the screen with the color just set. erease the previous frame
}

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


//Function

void display_col(column_t* pcol, app_t *app){
    SDL_Rect dest_bottom_up;
    SDL_Rect dest_bottom_down;
    SDL_Rect dest_top_up;
    SDL_Rect dest_top_down;
    int tex_bottom_w, tex_bottom_h, tex_top_h,tex_top_w; 
    for(int i=0; i< NUM_COL; i++){
            if (COL_WIDTH > pcol[i].len) {
                if (pcol[i].trim < COL_BOTTOM_WIDTH) {
                    pcol[i].trim++;
                }
            } else {
                pcol[i].trim = 0; // reset cuando está a ancho completo
            }
            //Displays the top of the column of the sup part
            dest_top_up.x=pcol[i].x;
            dest_top_up.y=pcol[i].y-COL_TOP_HIGH;
            dest_top_up.w=COL_WIDTH-(COL_WIDTH-pcol[i].len);
            dest_top_up.h=COL_TOP_HIGH ;
            SDL_RenderCopy(app->renderer, pcol[i].texture_col_top, NULL, &dest_top_up);
            
            //Displays the bottom of the column of the sup part
            dest_bottom_up.x=pcol[i].x-((COL_BOTTOM_WIDTH-COL_WIDTH)/2);
            dest_bottom_up.y=0;
            dest_bottom_up.w=COL_BOTTOM_WIDTH - pcol[i].trim ;   
            dest_bottom_up.h=pcol[i].y-COL_TOP_HIGH;
            SDL_RenderCopy(app->renderer, pcol[i].texture_col_bottom, NULL, &dest_bottom_up);

            //Displays the top of the column of the inferior part
            dest_top_down.x=pcol[i].x;
            dest_top_down.y=pcol[i].y + HOLE_HEIGHT;
            dest_top_down.w=COL_WIDTH-(COL_WIDTH-pcol[i].len);
            dest_top_down.h=COL_TOP_HIGH;
            SDL_RenderCopy(app->renderer, pcol[i].texture_col_top, NULL, &dest_top_down);

            //Displays the bottom of the column of the inferior part
            dest_bottom_down.x=pcol[i].x-((COL_BOTTOM_WIDTH-COL_WIDTH)/2);
            dest_bottom_down.y=pcol[i].y + HOLE_HEIGHT + COL_TOP_HIGH;
            dest_bottom_down.w=COL_BOTTOM_WIDTH - pcol[i].trim;
            dest_bottom_down.h = GAME_HEIGHT - (pcol[i].y + HOLE_HEIGHT + COL_TOP_HIGH);
            SDL_RenderCopy(app->renderer, pcol[i].texture_col_bottom, NULL, &dest_bottom_down);
        
           
    }
}
/*void display_upper_line(menu_t menu) {
    mvprintw(0, 0, "Lives: ");
    for (int i = 0; i < menu.lives; i++) {
        printw(" <3 "); // harts
    }
    char str[20];
    sprintf(str, "Score: %d", menu.score);  // Convierte el número en texto
    mvprintw(0, GAME_WIDTH-strlen(str)-1, "%s", str);
}

void displayy_bird(bird_t * bird, int ch){
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