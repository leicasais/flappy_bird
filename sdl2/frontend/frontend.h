#include "backend.h"
#ifndef FRONTEND_H
#define FRONTEND_H

    //Prototipes

    void prepareScene(app_t *app);
    void draw_bird(bird_t *bird, app_t *app);
    void draw_col(column_t* pcol, app_t *app);
    static void draw_tiled_segment(SDL_Renderer *r, SDL_Texture *tex, int x, int src_x, int y, int w, int src_w, int h);
    void draw_background(background_t *background, app_t *app);

    // UI init/shutdown (texto opcional con SDL_ttf; si no está, igual renderiza “sin texto”)
    int  ui_init(app_t *app, const char *ttf_path, int pt_size);
    void ui_shutdown(void);

    // Menús
    void render_main_menu(app_t *app, menu_t *menu, int w, int h);
    void render_pause_menu(app_t *app, menu_t *menu, int w, int h);
    void render_game_over(app_t *app, menu_t *menu, int w, int h);

    // HUD opcional
    void render_game_hud(app_t *app, menu_t *menu);
#endif