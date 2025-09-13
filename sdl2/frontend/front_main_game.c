#include "front_main_game.h"
#include "render_helpers.h"

void draw_bird(bird_t *bird, app_t *app, int shake_x, int shake_y){
    int fly_frames[] = {0, 1, 2, 1};
    //&srcRect the position and size in pixeles that you want to cut off the img
    SDL_Rect scr_rect;
    int frame_index = fly_frames[bird->current_frame];
    scr_rect.x = (frame_index) * HITBOX_X;          //gives the index of the column of the bird matrix
    scr_rect.y = 0;
    scr_rect.w = HITBOX_X;
    scr_rect.h = HITBOX_Y;

    // &destRect sets the size of the cut img and where to plaice it in the screen
    SDL_Rect dest_rect = {.x = bird->x_l + shake_x, .y = bird->y_top + shake_y, .w = bird->w, .h = bird->h};
    SDL_RenderCopyEx(app->renderer, bird->texture, &scr_rect, &dest_rect, bird->vel_y*3,NULL, SDL_FLIP_NONE);       // Copies a part of a texture (src_rect) and draws it in the screen using the active render
}
void explotion_render(ExplotionAnim *e, SDL_Renderer *r){
    if (!e || !e->playing) return;
    SDL_Texture *tex = e->frames[e->cur];
    if (!tex) return;

    SDL_Rect dst = { e->x , e->y , e->w*5, e->h*5 };
    SDL_RenderCopy(r, tex, NULL, &dst);
}
void draw_resurecting_bird(bird_t *bird, app_t *app, int frame, int shake_x, int shake_y){
    SDL_Rect scr_rect;
    scr_rect.x = frame * HITBOX_X;          //gives the index of the column of the bird matrix
    scr_rect.y = 0;
    scr_rect.w = HITBOX_X;
    scr_rect.h = HITBOX_Y;

    // &destRect sets the size of the cut img and where to plaice it in the screen
    SDL_Rect dest_rect = {.x = bird->x_l + shake_x, .y = bird->y_top+ shake_y, .w = bird->w, .h = bird->h};
    SDL_RenderCopyEx(app->renderer, bird->tex_resurrection, &scr_rect, &dest_rect, bird->vel_y*3,NULL, SDL_FLIP_NONE);       // Copies a part of a texture (src_rect) and draws it in the screen using the active render

}

void draw_col(column_t* pcol, app_t *app, screen_dim_t *screen_dim, int shake_x, int shake_y){
    for(int i=0; i< screen_dim->NUM_COL; i++){
        int src_x;
        int px_per_len;
        if(pcol[i].x != OUTSIDE) {
            px_per_len= (COL_PX_W/screen_dim->COL_WIDTH)* (pcol[i].len);
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
            int up_w=screen_dim->COL_WIDTH-(screen_dim->COL_WIDTH-pcol[i].len);
            int up_h=pcol[i].y;
            draw_tiled_segment(app->renderer, pcol[i].texture_up, up_x+ shake_x,src_x, up_y+shake_y, up_w,px_per_len, up_h); 
                
            //Displays the bottom of the column of the inferior part
            int down_x=(int)pcol[i].x;
            int down_y=pcol[i].y + screen_dim->HOLE_HEIGHT;
            int down_w=screen_dim->COL_WIDTH-(screen_dim->COL_WIDTH-pcol[i].len);
            int down_h = (screen_dim->GAME_HEIGHT-screen_dim->TILE_HIGHT-1) - (pcol[i].y + screen_dim->HOLE_HEIGHT);
            draw_tiled_segment(app->renderer, pcol[i].texture_down, down_x+ shake_x,src_x, down_y+shake_y, down_w,px_per_len , down_h);
        }
    }
}

void draw_background(background_t *background, app_t *app, screen_dim_t *screen_dim, int shake_x, int shake_y){
    SDL_Rect dest_tile = {.x = 0+ shake_x, .y= screen_dim->GAME_HEIGHT-screen_dim->TILE_HIGHT-1 + shake_y, .w= screen_dim->GAME_WIDTH, .h = screen_dim->TILE_HIGHT};
    SDL_RenderCopy(app->renderer, background->tile_tex, NULL, &dest_tile);

    SDL_Rect dest_clouds = {.x = 0+ shake_x, .y= 0, .w= screen_dim->GAME_WIDTH, .h = screen_dim->GAME_HEIGHT};
    SDL_RenderCopy(app->renderer, background->clouds, NULL, &dest_clouds);

}


void draw_hearts(app_t *app,menu_t *menu , int shake_x, int shake_y){
    
    SDL_Rect heart_rect = { .y = 4 + shake_y, .w= menu->heart_w, .h = menu->heart_h};
    for(int i=0; i<3; i++){
        heart_rect.x=8+ i*(menu->heart_w+menu->heart_w/4)+ shake_x;
        if(menu->lives >i){         //Case live is full
            SDL_RenderCopy(app->renderer, menu->full_heart_tex, NULL, &heart_rect);
        }
        else{
            SDL_RenderCopy(app->renderer, menu->empty_heart_tex, NULL, &heart_rect);
        }
    }
    
}


void render_game_hud(app_t *app, menu_t *menu, screen_dim_t *screen_dim, int shake_x, int shake_y){
        //Draw the points collected
    char buf[32]; 
    snprintf(buf, sizeof(buf), "Score: %d", menu->score);
    SDL_Color c = { 0, 0, 0, 255 };
    //the size of the hole text so it can be left centered
    int text_w = shake_x, text_h = shake_y;
    if (TTF_SizeText(app->font, buf, &text_w, &text_h) != 0) {
        fprintf(stderr, "TTF_SizeText error: %s\n", TTF_GetError());
        return;
    }
    draw_text_left(app->renderer, app->font, buf, c, screen_dim->GAME_WIDTH- text_w -10,8, NULL, NULL );
}

void display_resurecting(bird_t * bird, menu_t *menu, app_t *app,int shake_x, int shake_y){
    static int time = 0;
    int blink_on = (time < 5);
    SDL_Rect heart_rect = { .y = 4 +shake_y, .w= menu->heart_w, .h = menu->heart_h};
    for (int i = 0; i < 3; i++) {
        heart_rect.x = 8 + i * (menu->heart_w + menu->heart_w/4) + shake_x;

        if (i != menu->lives) {
            //show normally all lives except the current one that is being lost
            if (menu->lives > i) SDL_RenderCopy(app->renderer, menu->full_heart_tex, NULL, &heart_rect);
            else                 SDL_RenderCopy(app->renderer, menu->empty_heart_tex, NULL, &heart_rect);
        } else {
            // blinking heart
            if (blink_on) SDL_RenderCopy(app->renderer, menu->full_heart_tex, NULL, &heart_rect);
            else          SDL_RenderCopy(app->renderer, menu->empty_heart_tex, NULL, &heart_rect);
        }
    }
    if(bird->vel_y<0){
        draw_resurecting_bird(bird, app, 1,shake_x,shake_y);
    }
    else{
        draw_resurecting_bird(bird, app, 0,shake_x,shake_y);
    } 
    time = (time + 1) % 10;
    
    
}


