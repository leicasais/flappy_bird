#include "backend.h"
#include "frontend.h"
//Variables Globales
int GAME_WIDTH;              //Stores the WIDTH of the screen
int GAME_HEIGHT;             //Stores the HEIGHT of the screen
int HOLE_HEIGHT;        //Stores the Height of the holes
int NUM_COL;            //Stores the number of columns including the ones OUTSIDE the screen
int COL_WIDTH;          //Stores the WIDTH of the columns
int COL_BOTTOM_WIDTH;
int COL_TOP_HIGH;
int SPACE;              //Stores the space between the columns
int BIRD_SCALE=4;       // Stores the scale of the bird from the real size of the img

column_t* column;       //stores all directions of the columns, they are not in order


int main(void){
    bird_t bird; //init bird
    menu_t menu ={.state=MAIN_MENU};
    app_t app;
    init_parameters();      //iniialices the first parameters that are needed to inicialice the obj 
    column = malloc(sizeof(column_t) * NUM_COL);

    memset(&app, 0, sizeof(app));       //inicialices all the values of app in 0
    initSDL(&app);

    init(column, &bird, &menu, &app);   //inicialices all the objects of the game

    //game loop
    while (1){
        //game logic
        prepareScene(&app);
        SDL_Event event;    // hold events (like key presses, mouse clicks, window close, etc.)
        Uint32 last_time = SDL_GetTicks();
        Uint32 current_time;

        //key-driven actions
        while (SDL_PollEvent(&event)) {     //pulls events from SDL’s event queue one by one
            switch (event.type)
            {
                case SDL_QUIT:      //This event happens when the user clicks the window’s "X" close button
                    exit(0);
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_SPACE:
                            bird.vel_y = -8.0;
                            break;
                        
                        default:
                            break;
                        }
                    break;
                default:
                    break;
            }
        }
        // Alltime actions
        col_mov(column);
        bird_fall(&bird);
        update_bird_animation(&bird);

        //game display
        display_col(column, &app);
        display_bird(&bird, &app);
        SDL_RenderPresent(app.renderer);       //“I’m done drawing everything — now show it on the screen!”

        SDL_Delay(16);      //16 ms == 62 FPS 
    }

    free(column);
    cleanupSDL(&app, &bird, column);
    return 0;
}