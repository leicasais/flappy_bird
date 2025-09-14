#include "running_fun.h"

/************************************ 
         COLUMN FUNCTIONS
************************************/

static float col_scroll_accum = 0.0f;

void col_mov(column_t* pcol, screen_dim_t *screen_dim, menu_t *pmenu){
    // 1) accumulate speed and convert to integer steps
    col_scroll_accum += pcol->col_speed;             // pcol->col_speed lives in column[0]
    int step = (int)col_scroll_accum;
    if (step <= 0){
        return;
    }
    col_scroll_accum -= step;

    // 2) move and crop/grow the visible part accordingly
    for (int i = 0; i < screen_dim->NUM_COL; i++){
        if (pcol[i].len <= 0){
            continue; // inactive
        }              

        // move to the left
        int xi = (int)pcol[i].x - step;
        pcol[i].x = xi;

        if (xi < 0){
            // went out on the left: trim the portion that went off-screen
            int over = -xi;                           // how much it went past the border
            pcol[i].x = 0;
            pcol[i].len -= over;                      // trim the visible portion
            if (pcol[i].len <= 0){
                pcol[i].len = 0;
                pcol[i].x = OUTSIDE;               // deactivate
                pcol[i].col_speed_y = 0;
            }
        } 
        else if (pcol[i].len < screen_dim->COL_WIDTH){
            // entering from the right: gradually reveal the texture
            pcol[i].len += step;
            if (pcol[i].len > screen_dim->COL_WIDTH){
                pcol[i].len = screen_dim->COL_WIDTH;
            }
        }
    }

    // 3) is there room on the right to spawn a new column?
    int rightmost_right = 0;                          // farthest right edge
    for (int j = 0; j < screen_dim->NUM_COL; j++){
        if (pcol[j].len <= 0){
            continue;
        }
        int r = (int)pcol[j].x + pcol[j].len;
        if (r > rightmost_right){
            rightmost_right = r;
        }
    }

    // If there is free space (rightmost edge + SPACE < window edge), activate a new one
    if (rightmost_right + screen_dim->SPACE <= screen_dim->GAME_WIDTH - 1){
        for (int i = 0; i < screen_dim->NUM_COL; i++){
            if (pcol[i].len == 0){
                pcol[i].x = screen_dim->GAME_WIDTH - 1;        // glued to the right edge
                pcol[i].y = rand_hole(screen_dim);
                pcol[i].len = 1;                     // starts small, enters gradually
                pcol[i].trim = 0;
                int n = pmenu->dificulty;                 // re-enable score counting
                if ((rand() % 100) < n){
                    float vel = 0.4f + 0.05f * (float)n * ((float)(rand()%61)/100.0f); // 
                    pcol[i].col_speed_y = (rand()%2) ? vel : -vel;
                }
                break; 
            }
        }

    }

    // Part for the "Dynamic Columns"
    int minY = 2;
    const int maxY = screen_dim->GAME_HEIGHT - screen_dim->TILE_HIGHT - screen_dim->HOLE_HEIGHT - 1;
    for (int i = 0; i < screen_dim->NUM_COL; i++){
        if (pcol[i].len <= 0 || pcol[i].x == OUTSIDE){
            continue;
        }
        float speed_y = pcol[i].col_speed_y;
        if (speed_y != 0.0f){
            pcol[i].y += speed_y;
            // bounce within valid hole boundaries
            if (pcol[i].y < (float)minY){
                pcol[i].y = (float)minY;
                pcol[i].col_speed_y = -speed_y;
            } else if (pcol[i].y > (float)maxY){
                pcol[i].y = (float)maxY;
                pcol[i].col_speed_y = -speed_y;
            }
        }
    }

}

void col_reset_scroll(void){
    col_scroll_accum = 0.0f;
}

int rand_hole(screen_dim_t *screen_dim){ // returns a random y coord for the beginning of the hole  NOTE -> min + rand() % (max - min + 1);      
    const int margin_top = 2; 
    int minY = margin_top;
    int maxY = screen_dim->GAME_HEIGHT - screen_dim->TILE_HIGHT - screen_dim->HOLE_HEIGHT - 1; // correct limit
    if (maxY < minY){
        // impossible to place the hole: adjust HOLE_HEIGHT or margins
        maxY = minY;
    }
    return minY + rand() % (maxY - minY + 1); // range [minY, maxY]
}



/************************************ 
         BIRD FUNCTIONS
*************************************/

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
    int animation_speed = 1000 / 10; // 100 FPS

    if ( (SDL_GetTicks()-(bird->last_frame_time)) > animation_speed) {  // Update bird animation
        bird->current_frame = (bird->current_frame + 1) % num_frames;
        bird->last_frame_time = SDL_GetTicks();
    }
}

void bird_jump(bird_t* bird){
    float jump_displacement=-0.1; // how far the bird would jump without gravity
    bird->y_top+= jump_displacement;
    bird->y_bottom+= jump_displacement;
}

void bird_fall(bird_t* bird, screen_dim_t *screen_dim){
    bird->vel_y += bird->gravity_y;
    bird->y_top +=bird->vel_y;        // the bird experiences gravity both with and without the jump
    bird->y_bottom+=bird->vel_y;

    // Prevent the bird from going out of the screen 
    if (bird->y_bottom > (screen_dim->GAME_HEIGHT - 1 - screen_dim->TILE_HIGHT)){
        bird->y_bottom = (screen_dim->GAME_HEIGHT - screen_dim->TILE_HIGHT)-1;
        bird->y_top = (screen_dim->GAME_HEIGHT - screen_dim->TILE_HIGHT)-1-(HITBOX_Y/(bird->scale));
        bird->vel_y = 0;
    }
    else if (bird->y_top < 0){
        bird->y_top = 1;
        bird->y_bottom =(HITBOX_Y/(bird->scale))+1;
        bird->vel_y = 0;
    }
}