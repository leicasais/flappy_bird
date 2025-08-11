#include "frontend.h"
#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int HOLE_HEIGHT;
extern int TILE_HIGHT;
extern int NUM_COL;
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;

//windows
void prepareScene(app_t *app)   //Prepares a clean background for drawing the next frame.
{
    SDL_SetRenderDrawColor(app->renderer,106, 138, 153, 255);   //Sets the color that SDL will use when clearing or drawing, SDL_SetRenderDrawColor(app.renderer, R, G, B, A); A: opacidad (255 ultra opaco)
    SDL_RenderClear(app->renderer);         //Clears the screen with the color just set. erease the previous frame
}

void draw_bird(bird_t *bird, app_t *app){
    int fly_frames[] = {0, 6, 6, 7};
    //&srcRect the position and size in pixeles that you want to cut off the img
    SDL_Rect scr_rect;
    int frame_index = fly_frames[bird->current_frame];
    scr_rect.x = (frame_index % 4) * HITBOX_X;          //gives the index of the column of the bird matrix
    scr_rect.y = (frame_index / 4) * HITBOX_Y;
    scr_rect.w = HITBOX_X;
    scr_rect.h = HITBOX_Y;

    // &destRect sets the size of the cut img and where to plaice it in the screen
    SDL_Rect dest_rect = {.x=bird->x_l, .y=bird->y_top, .w= (bird->x_r - bird->x_l), .h= bird->y_bottom - bird->y_top} ; 
    SDL_RenderCopy(app->renderer, bird->texture, &scr_rect, &dest_rect);       // Copies a part of a texture (src_rect) and draws it in the screen using the active render



}

// sin estirar en vertical: repite la textura en bloques y recorta el último si hace falta.
static void draw_tiled_segment(SDL_Renderer *r, SDL_Texture *tex, int x, int src_x, int y, int w, int src_w, int h)
{
    if (h <= 0 || w <= 0 || !tex) return;

    int texW, texH;
    SDL_QueryTexture(tex, NULL, NULL, &texW, &texH);

    int drawn = 0;
    while (drawn < h) {
        int chunk = (h - drawn < texH) ? (h - drawn) : texH;

        SDL_Rect src = {src_x, 0, src_w, chunk};        // recorto solo lo que entra
        SDL_Rect dst = {x, y + drawn, w, chunk};   // mismo alto que src -> sin estirar verticalmente
        SDL_RenderCopy(r, tex, &src, &dst);

        drawn += chunk;
    }
}
void draw_col(column_t* pcol, app_t *app){

    for(int i=0; i< NUM_COL; i++){
            int src_x;
            int px_per_len;
            if(pcol[i].x != OUTSIDE){
                px_per_len= (COL_PX_W/COL_WIDTH)* (pcol[i].len);
                SDL_Rect src_rect = {.y =0, .h =COL_PX_H, .w=px_per_len};
                if(pcol[i].x==0 ){
                    src_x= COL_PX_W-(px_per_len);
                }
                else{
                    src_x=0;
                }

                //Displays the top of the column of the sup part
                int up_x=pcol[i].x;
                int up_y=0;
                int up_w=COL_WIDTH-(COL_WIDTH-pcol[i].len);
                int up_h=pcol[i].y;
                draw_tiled_segment(app->renderer, pcol[i].texture_up, up_x,src_x, up_y, up_w,px_per_len, up_h);

                //Displays the bottom of the column of the inferior part
                int down_x=pcol[i].x;
                int down_y=pcol[i].y + HOLE_HEIGHT;
                int down_w=COL_WIDTH-(COL_WIDTH-pcol[i].len);
                int down_h = (GAME_HEIGHT-TILE_HIGHT-1) - (pcol[i].y + HOLE_HEIGHT);
                draw_tiled_segment(app->renderer, pcol[i].texture_down, down_x,src_x, down_y, down_w,px_per_len , down_h);
            }
        
           
    }
}


void draw_background(background_t *background, app_t *app){
    SDL_Rect dest_tile = {.x = 0, .y= GAME_HEIGHT-TILE_HIGHT-1, .w= GAME_WIDTH, .h = TILE_HIGHT};
    SDL_RenderCopy(app->renderer, background->tile_tex, NULL, &dest_tile);
    
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