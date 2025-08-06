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

//Bird funcions
void update_bird_animation(bird_t *bird){
    int num_frames = 4;
    int animation_speed = 1000 / 20; // 50 ms per frame, 20 FPS

    if ( (SDL_GetTicks()-(bird->last_frame_time)) > animation_speed) {  //Update bird animation
        bird->current_frame = (bird->current_frame + 1) % num_frames;
        bird->last_frame_time = SDL_GetTicks();
    }

}
void bird_jump(bird_t* bird){
    float jump_displacement=-0.1; //how far the bird would jump without gravity
    bird->y_top+= jump_displacement;
    bird->y_bottom+= jump_displacement;
}
void bird_fall(bird_t* bird){
    bird->vel_y += bird->gravity_y;
    bird->y_top +=bird->vel_y;        //the bird suffers the efect of gravity with and without the jump
    bird->y_bottom+=bird->vel_y;

    // Don't allow the bird going out of the screen 
    if (bird->y_bottom > (GAME_HEIGHT-1)) {
        bird->y_bottom = GAME_HEIGHT-1;
        bird->y_top = GAME_HEIGHT-1-(HITBOX_Y*BIRD_SCALE);
        bird->vel_y = 0;
    }
    else if (bird->y_top < 1) {
        bird->y_top = 1;
        bird->y_bottom =(HITBOX_Y*BIRD_SCALE)+1;
        bird->vel_y = 0;
    }
}

