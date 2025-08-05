#include "frontend.h"
#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int HOLE_HEIGHT;
extern int NUM_COL;
extern int COL_WIDTH;
extern int SPACE;
extern float GRAVITY;
extern float JUMP_VEL;   

void prepareScene(app_t *app)   //Prepares a clean background for drawing the next frame.
{
    SDL_SetRenderDrawColor(app->renderer,128, 0, 128, 255);   //Sets the color that SDL will use when clearing or drawing, SDL_SetRenderDrawColor(app.renderer, R, G, B, A); A: opacidad (255 ultra opaco)
    SDL_RenderClear(app->renderer);         //Clears the screen with the color just set. erease the previous frame
}

void presentScene(app_t *app){
    SDL_RenderPresent(app->renderer);       //“I’m done drawing everything — now show it on the screen!”
}