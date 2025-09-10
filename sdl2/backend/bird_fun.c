#include "backend.h"

//Bird funcions
void update_bird_animation(bird_t *bird){
    if(bird->vel_y < 0){
        bird_flying(bird);
    }
    else{
        if(bird->current_frame != 2){
            bird->current_frame = 2;
        }
    }
}

void bird_flying(bird_t *bird){
    int num_frames = 4;
    int animation_speed = 1000 / 10; // 10 FPS

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
void bird_fall(bird_t* bird, screen_dim_t *screen_dim){
    bird->vel_y += bird->gravity_y;
    bird->y_top +=bird->vel_y;        //the bird suffers the efect of gravity with and without the jump
    bird->y_bottom+=bird->vel_y;

    // Don't allow the bird going out of the screen 
    if (bird->y_bottom > (screen_dim->GAME_HEIGHT - 1 - screen_dim->TILE_HIGHT)) {
        bird->y_bottom = (screen_dim->GAME_HEIGHT - screen_dim->TILE_HIGHT)-1;
        bird->y_top = (screen_dim->GAME_HEIGHT - screen_dim->TILE_HIGHT)-1-(HITBOX_Y/(bird->scale));
        bird->vel_y = 0;
    }
    else if (bird->y_top < 0) {
        bird->y_top = 1;
        bird->y_bottom =(HITBOX_Y/(bird->scale))+1;
        bird->vel_y = 0;
    }
}

