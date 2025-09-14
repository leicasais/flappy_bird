#ifndef FRONT_MAIN_GAME_H
#define FRONT_MAIN_GAME_H

    //Incluide Typedefs and Macros
    #include "structs.h"

    //main_game_display
    void draw_bird(bird_t *bird, app_t *app, int shake_x, int shake_y);
    void draw_resurecting_bird(bird_t *bird, app_t *app, int frame, int shake_x, int shake_y);
    void explotion_render(ExplotionAnim *e, SDL_Renderer *r);
    void draw_col(column_t* pcol, app_t *app, screen_dim_t *screen_dim, int shake_x, int shake_y);
    void draw_background(background_t *background, app_t *app, screen_dim_t *screen_dim, int shake_x, int shake_y);
    void draw_hearts(app_t *app,menu_t *menu , int shake_x, int shake_y);
    void render_game_hud(app_t *app, menu_t *menu, screen_dim_t *screen_dim, int shake_x, int shake_y);
    void display_resurecting(bird_t * bird, menu_t *menu, app_t *app,int shake_x, int shake_y);

#endif