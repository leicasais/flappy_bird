#include "backend.h"
#ifndef FRONTEND_H
#define FRONTEND_H

    // Render base
    void prepareScene(app_t *app);
    void draw_bird(bird_t *bird, app_t *app);
    void draw_col(column_t* pcol, app_t *app);
    void draw_background(background_t *background, app_t *app);

    // ---- UI / Menú ----
    void draw_text_center(SDL_Renderer* r, TTF_Font* font, const char* text, SDL_Color color, int cx, int y, int* out_w, int* out_h);

    void draw_menu_list(SDL_Renderer *r, int w, int h, const char **options, int n, int selected, const char *title, const char *subtitle);

    void render_main_menu(app_t *app, menu_t *menu, int w, int h);
    void render_game_hud(app_t *app, menu_t *menu);

#endif
