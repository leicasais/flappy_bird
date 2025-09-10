#ifndef INIT_FUN_H
#define INIT_FUN_H

    //Incluide Typedefs and Macros
    #include "structs.h"
    
    //init_fun.c
    void set_parameters(screen_dim_t *screen_dim);
    void init_parameters(screen_dim_t *screen_dim);
    int rand_y_pos(float bird_size, int heart_h,screen_dim_t *screen_dim);
    void init(column_t* pcol, bird_t *bird, menu_t *menu, screen_dim_t *screen_dim);
    void init_tex(column_t* column, bird_t *bird, menu_t *menu, app_t *app, background_t *background, screen_dim_t *screen_dim); 
    void initSDL(app_t *app, screen_dim_t *screen_dim);
    SDL_Texture *loadTexture(char *filename, app_t *app);
    void cleanupSDL(app_t *app, bird_t *bird, column_t *column, background_t *background, menu_t *menu, screen_dim_t *screen_dim);
    void set_bird_skin(bird_t *bird, app_t *app, int idx);

#endif