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

void draw_bird(bird_t *bird, app_t *app){
    int fly_frames[] = {0, 1, 2, 1};
    //&srcRect the position and size in pixeles that you want to cut off the img
    SDL_Rect scr_rect;
    int frame_index = fly_frames[bird->current_frame];
    scr_rect.x = (frame_index) * HITBOX_X;          //gives the index of the column of the bird matrix
    scr_rect.y = 0;
    scr_rect.w = HITBOX_X;
    scr_rect.h = HITBOX_Y;

    // &destRect sets the size of the cut img and where to plaice it in the screen
    SDL_Rect dest_rect = {.x = (int)bird->x_l, .y = (int)bird->y_top, .w = (int)(bird->x_r - bird->x_l), .h = (int)(bird->y_bottom - bird->y_top)};
    SDL_RenderCopyEx(app->renderer, bird->texture, &scr_rect, &dest_rect, bird->vel_y*3,NULL, SDL_FLIP_NONE);       // Copies a part of a texture (src_rect) and draws it in the screen using the active render
}

void draw_resurecting_bird(bird_t *bird, app_t *app, int frame){
    SDL_Rect scr_rect;
    scr_rect.x = frame * HITBOX_X;          //gives the index of the column of the bird matrix
    scr_rect.y = 0;
    scr_rect.w = HITBOX_X;
    scr_rect.h = HITBOX_Y;

    // &destRect sets the size of the cut img and where to plaice it in the screen
    SDL_Rect dest_rect = {.x = (int)bird->x_l, .y = (int)bird->y_top, .w = (int)(bird->x_r - bird->x_l), .h = (int)(bird->y_bottom - bird->y_top)};
    SDL_RenderCopyEx(app->renderer, bird->tex_resurrection, &scr_rect, &dest_rect, bird->vel_y*3,NULL, SDL_FLIP_NONE);       // Copies a part of a texture (src_rect) and draws it in the screen using the active render

}

void draw_col(column_t* pcol, app_t *app){
    for(int i=0; i< NUM_COL; i++){
        int src_x;
        int px_per_len;
        if(pcol[i].x != OUTSIDE) {
            px_per_len= (COL_PX_W/COL_WIDTH)* (pcol[i].len);
            SDL_Rect src_rect = {.y =0, .h =COL_PX_H, .w=px_per_len};
            if((int)pcol[i].x==0 ) {
                src_x= COL_PX_W-(px_per_len);
            }
            else{
                src_x=0;
            }
                
            //Displays the top of the column of the sup part
            int up_x=(int)pcol[i].x;
            int up_y=0;
            int up_w=COL_WIDTH-(COL_WIDTH-pcol[i].len);
            int up_h=pcol[i].y;
            draw_tiled_segment(app->renderer, pcol[i].texture_up, up_x,src_x, up_y, up_w,px_per_len, up_h); 
                
            //Displays the bottom of the column of the inferior part
            int down_x=(int)pcol[i].x;
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


void draw_hearts(app_t *app,menu_t *menu ){
    SDL_Rect heart_rect = { .y = 4, .w= menu->heart_w/4, .h = menu->heart_h/4};
    for(int i=0; i<3; i++){
        heart_rect.x=8+ i*(menu->heart_w/4+menu->heart_w/9);
        if(menu->lives >i){         //Case live is full
            SDL_RenderCopy(app->renderer, menu->full_heart_tex, NULL, &heart_rect);
        }
        else{
            SDL_RenderCopy(app->renderer, menu->empty_heart_tex, NULL, &heart_rect);
        }
    }
    
}


void render_game_hud(app_t *app, menu_t *menu){
        //Draw the points collected
    char buf[32]; 
    snprintf(buf, sizeof(buf), "Score: %d", menu->score);
    SDL_Color c = { 250, 250, 250, 255 };
    //the size of the hole text so it can be left centered
    int text_w = 0, text_h = 0;
    if (TTF_SizeText(app->font, buf, &text_w, &text_h) != 0) {
        fprintf(stderr, "TTF_SizeText error: %s\n", TTF_GetError());
        return;
    }
    draw_text_left(app->renderer, app->font, buf, c, GAME_WIDTH- text_w -10,8, NULL, NULL );
}

void display_resurecting(bird_t * bird, menu_t *menu, app_t *app){
    static int time=0;
    SDL_Rect heart_rect = { .y = 4, .w= menu->heart_w/4, .h = menu->heart_h/4};
        for(int i=0; i<3; i++){
            heart_rect.x=8+ i*(menu->heart_w/4+menu->heart_w/9);
            if(i != menu->lives){     //show normally all lives except the current one that is being lost
                if(menu->lives >i){         //Case live is full
                    SDL_RenderCopy(app->renderer, menu->full_heart_tex, NULL, &heart_rect);
                }
                else{
                    SDL_RenderCopy(app->renderer, menu->empty_heart_tex, NULL, &heart_rect);
                }
            }
            else{
                if(time<5){
                    SDL_RenderCopy(app->renderer, menu->full_heart_tex, NULL, &heart_rect);
                }
                else{
                    SDL_RenderCopy(app->renderer, menu->empty_heart_tex, NULL, &heart_rect);
                }
            }
        }
    if(bird->vel_y<0){
        draw_resurecting_bird(bird, app, 1);
    }
    else{
        draw_resurecting_bird(bird, app, 0);
        
    if(time==10) {
        time=0;
    }
    else{
        time++;
    }
    }  
    
}


