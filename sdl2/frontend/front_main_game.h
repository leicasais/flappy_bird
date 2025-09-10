#ifndef FRONT_MAIN_GAME_H
#define FRONT_MAIN_GAME_H

    //Incluide Typedefs and Macros
    #include "structs.h"

    //main_game_display
    void bird_flying(bird_t *bird);
    void draw_bird(bird_t *bird, app_t *app);
    void draw_resurecting_bird(bird_t *bird, app_t *app, int frame);
    void display_resurecting(bird_t * bird, menu_t *menu, app_t *app);
    void draw_col(column_t* pcol, app_t *app, screen_dim_t *screen_dim);
    void draw_background(background_t *background, app_t *app, screen_dim_t *screen_dim);
    void draw_hearts(app_t *app,menu_t *menu );
    void render_game_hud(app_t *app, menu_t *menu, screen_dim_t *screen_dim);
#endif