#include "frontend.h"
#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int HOLE_HEIGHT;
extern int TILE_HIGHT;
extern int NUM_COL;
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;

//windows
void prepareScene(app_t *app){   //Prepares a clean background for drawing the next frame.
    SDL_SetRenderDrawColor(app->renderer,106, 138, 153, 255);   //Sets the color that SDL will use when clearing or drawing, SDL_SetRenderDrawColor(app.renderer, R, G, B, A); A: opacidad (255 ultra opaco)
    SDL_RenderClear(app->renderer);         //Clears the screen with the color just set. erease the previous frame
}

void draw_tiled_segment(SDL_Renderer *r, SDL_Texture *tex, int x, int src_x, int y, int w, int src_w, int h){
    if (h <= 0 || w <= 0 || !tex) {
        return;
    }

    int texW, texH;
    SDL_QueryTexture(tex, NULL, NULL, &texW, &texH);

    int drawn = 0;
    while (drawn < h) {
        int chunk = (h - drawn < texH) ? (h - drawn) : texH;

        SDL_Rect src = {src_x, 0, src_w, chunk};        // recorto solo lo que entra
        SDL_Rect dst = {x, y + drawn, w, chunk};   // mismo alto que src -> sin estirar verticalmente
        SDL_RenderCopy(r, tex, &src, &dst);

        drawn += chunk;
    }
}

//text fun

SDL_Texture* make_text(SDL_Renderer* r, TTF_Font* font, const char* txt, SDL_Color color, int* w, int* h) {
    SDL_Surface* s = TTF_RenderUTF8_Blended(font, txt, color);
    if (!s) { 
        SDL_Log("TTF_Render FAIL: %s", TTF_GetError()); 
        return NULL; 
    }
    SDL_Texture* t = SDL_CreateTextureFromSurface(r, s);
    if (w) { 
        *w = s->w; if (h) *h = s->h;
    }
    SDL_FreeSurface(s);
    SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
    return t;
}

void draw_text_center(SDL_Renderer* r, TTF_Font* font, const char* text, SDL_Color color, int cx, int y, int* out_w, int* out_h) {
    int w=0, h=0;
    SDL_Texture* tex = make_text(r, font, text, color, &w, &h);
    if (!tex) {
        return;
    }
    SDL_Rect dst = { cx - w/2, y, w, h };
    SDL_RenderCopy(r, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
    if (out_w) {
        *out_w = w; if (out_h) *out_h = h;
    }
}

void draw_text_left(SDL_Renderer* r, TTF_Font* font, const char* text, SDL_Color color, int x, int y, int* out_w, int* out_h) {
    int w=0, h=0;
    SDL_Texture* tex = make_text(r, font, text, color, &w, &h);
    if (!tex) {
        return;
    }
    SDL_Rect dst = { x, y, w, h };
    SDL_RenderCopy(r, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
    if (out_w) {
        *out_w = w; if (out_h) *out_h = h;
    }
}