#include "backend.h"
#ifndef FRONTEND_H
#define FRONTEND_H

    // --- Layout del panel (ajustes) ---
    #define PANEL_W_RATIO     0.58f   
    #define PANEL_H_RATIO     0.78f   
    #define PANEL_BOTTOM_PAD  52      
    #define OPTION_LINE_H        32    
    #define OPTION_GAP           16    
    #define OPTION_HILIGHT_PAD_Y 8     
        
    #define LIST_OPTIONS_GAP  18   // espacio fijo entre la lista y las opciones
    #define HILIGHT_INSET     20      // sangría del destacador

    //main_game_display
    void bird_flying(bird_t *bird);
    void draw_bird(bird_t *bird, app_t *app);
    void draw_resurecting_bird(bird_t *bird, app_t *app, int frame);
    void display_resurecting(bird_t * bird, menu_t *menu, app_t *app);
    void draw_col(column_t* pcol, app_t *app);
    void draw_background(background_t *background, app_t *app);
    void draw_hearts(app_t *app,menu_t *menu );

    // Render_helpers.c
    void prepareScene(app_t *app);
    void draw_tiled_segment(SDL_Renderer *r, SDL_Texture *tex, int x, int src_x, int y, int w, int src_w, int h);    // sin estirar en vertical: repite la textura en bloques y recorta el último si hace falta.
    void draw_text_center_scaled(SDL_Renderer* r, TTF_Font* font, const char* text, SDL_Color color, int cx, int y, float scale, int* out_w, int* out_h);
    void draw_text_left_scaled  (SDL_Renderer* r, TTF_Font* font, const char* text, SDL_Color color, int x,  int y, float scale, int* out_w, int* out_h);
    

    //text
    SDL_Texture* make_text(SDL_Renderer* r, TTF_Font* font, const char* txt, SDL_Color color, int* w, int* h);
    void draw_text_center(SDL_Renderer* r, TTF_Font* font, const char* text, SDL_Color color, int cx, int y, int* out_w, int* out_h);
    void draw_text_left(SDL_Renderer* r, TTF_Font* font, const char* text, SDL_Color color, int x, int y, int* out_w, int* out_h);
    

    // front_menu.c         ---- UI / Menú ----
    void draw_menu_list(SDL_Renderer *r, int w, int h, const char **options, int n, int selected, const char *title, const char *subtitle);
    void render_main_menu(app_t *app, menu_t *menu, int w, int h);
    void render_game_hud(app_t *app, menu_t *menu);
    void render_game_over(app_t *app, menu_t *menu, int w, int h);
    void render_pause_menu(app_t *app, menu_t *menu, int w, int h);


#endif
