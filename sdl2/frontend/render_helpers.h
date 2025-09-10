#ifndef RENDER_HELPERS_H
#define RENDER_HELPERS_H

    //Incluide Typedefs and Macros
    #include "structs.h"
    
    // Render_helpers.c
    void prepareScene(app_t *app);
    void draw_tiled_segment(SDL_Renderer *r, SDL_Texture *tex, int x, int src_x, int y, int w, int src_w, int h);    // sin estirar en vertical: repite la textura en bloques y recorta el último si hace falta.
    void draw_text_center_scaled(SDL_Renderer* r, TTF_Font* font, const char* text, SDL_Color color, int cx, int y, float scale, int* out_w, int* out_h);
    void draw_text_left_scaled  (SDL_Renderer* r, TTF_Font* font, const char* text, SDL_Color color, int x,  int y, float scale, int* out_w, int* out_h);

    //text
    SDL_Texture* make_text(SDL_Renderer* r, TTF_Font* font, const char* txt, SDL_Color color, int* w, int* h);
    void draw_text_center(SDL_Renderer* r, TTF_Font* font, const char* text, SDL_Color color, int cx, int y, int* out_w, int* out_h);
    void draw_text_left(SDL_Renderer* r, TTF_Font* font, const char* text, SDL_Color color, int x, int y, int* out_w, int* out_h);
    
    void window_input(void);

#endif