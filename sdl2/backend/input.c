#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int HOLE_HEIGHT;
extern int NUM_COL;
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;


void window_input(void){
    SDL_Event event;    // hold events (like key presses, mouse clicks, window close, etc.)

    while (SDL_PollEvent(&event)) {     //pulls events from SDL’s event queue one by one
        switch (event.type)
        {
            case SDL_QUIT:      //This event happens when the user clicks the window’s "X" close button
                exit(0);
                break;

            default:
                break;
        }
    }
}