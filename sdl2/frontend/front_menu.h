#ifndef FRONT_MENU_H
#define FRONT_MENU_H

    //Incluide Typedefs and Macros
    #include "structs.h"

    // front_menu.c         ---- UI / Menú ----
    void draw_menu_list(SDL_Renderer *r, int w, int h, const char **options, int n, int selected, const char *title, const char *subtitle);
    void render_main_menu(app_t *app, menu_t *menu, int w, int h);
    void render_dificulty_menu(app_t *app, menu_t *menu, int w, int h);
    void render_name_menu(app_t *app, menu_t *menu, int w, int h);
    void render_skin_menu(app_t *app, menu_t *menu, int w, int h);
    void render_game_over(app_t *app, menu_t *menu, int w, int h);
    void render_pause_menu(app_t *app, menu_t *menu, int w, int h);

#endif