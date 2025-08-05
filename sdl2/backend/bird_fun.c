#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int HOLE_HEIGHT;
extern int NUM_COL;
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;


//Bird funcions
void update_bird_animation(bird_t *bird){
    int num_frames = 4;
    int animation_speed = 1000 / 20; // 50 ms per frame, 20 FPS

    if ( (SDL_GetTicks()-(bird->last_frame_time)) > animation_speed) {  //Update bird animation
        bird->current_frame = (bird->current_frame + 1) % num_frames;
        bird->last_frame_time = SDL_GetTicks();
    }

}
/*
bird_mov(bird_t* bird, int ch){
    float jump_displacement=-0.1; //how far the bird would jump without gravity
    if(ch == ' '){
        bird->delta_y= jump_displacement;
    }
    bird->delta_y +=bird->gravity_y;
    bird->y+=bird->delta_y;

    // Don't allow the bird going out of the screen 
    if (bird->y > (GAME_HEIGHT-1)) {
        bird->y = GAME_HEIGHT-1;
        bird->delta_y=0;
    }
    else if (bird->y < 1) {
        bird->y = 1;
        bird->delta_y =0;
    }
    //Update hitbox
    bird->y_top=(int)(bird->y);
    bird->y_bottom=bird->y_top+1;
    
}
*/
