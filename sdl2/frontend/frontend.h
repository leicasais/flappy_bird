#include "backend.h"
#ifndef FRONTEND_H
#define FRONTEND_H

    //Prototipes

    void prepareScene(app_t *app);
    void draw_bird(bird_t *bird, app_t *app);
    void draw_col(column_t* pcol, app_t *app);
    static void draw_tiled_segment(SDL_Renderer *r, SDL_Texture *tex, int x, int src_x, int y, int w, int src_w, int h);
    void draw_background(background_t *background, app_t *app);

#endif