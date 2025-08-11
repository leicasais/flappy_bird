#include "backend/backend.h"
#include "frontend/frontend.h"

//Variables Globales
int GAME_WIDTH;              //Stores the WIDTH of the screen
int GAME_HEIGHT;             //Stores the HEIGHT of the screen
int TILE_HIGHT;          //stores the HEIGHT of the tile
int HOLE_HEIGHT;        //Stores the Height of the holes
int NUM_COL;            //Stores the number of columns including the ones OUTSIDE the screen
int COL_WIDTH;          //Stores the WIDTH of the columns
int SPACE;              //Stores the space between the columns

column_t* column;       //stores all directions of the columns, they are not in order


int main(void){
    bird_t bird; //init bird
    menu_t menu ={.state=BEGINING}; //TEMPORARY BEHAVIOR !! CHAGE IT TO MAIN_MENU
    app_t app;
    background_t background;
    init_parameters();      //iniialices the first parameters that are needed to inicialice the obj 
    column = malloc(sizeof(column_t) * NUM_COL);

    memset(&app, 0, sizeof(app));       //inicialices all the values of app in 0
    initSDL(&app);

    init(column, &bird, &menu, &app, &background);   //inicialices all the objects of the game

    //game loop
    while (1){
        //game logic
        prepareScene(&app);
        SDL_Event event;    // hold events (like key presses, mouse clicks, window close, etc.)
        Uint32 last_time = SDL_GetTicks();
        Uint32 current_time;
        // 2) Handle all pending events (never block the loop)
       
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    exit(0);
                    break;

                case SDL_KEYDOWN:
                if(menu.state == RUNING){
                     // Ignore key repeats so SPACE triggers once
                    if (event.key.repeat) break;
                    else if (event.key.keysym.sym == SDLK_SPACE) {
                        // Jump while running
                        bird.vel_y = -8.0f;
                    }
                    break;
                }
                if (menu.state == BEGINING && event.key.keysym.sym == SDLK_SPACE) {
                        // Start the game from the "Press SPACE" screen
                        menu.state = RUNING;
                }
            }
        }

        // 3) Update game logic at a fixed rate (can run 0..N times per frame)
            switch (menu.state) {
                case MAIN_MENU:
                    // TODO: animate menu or handle inputs if needed
                    break;

                case BEGINING:
                    // Idle pre-start animation (e.g., bird flapping)
                    update_bird_animation(&bird);
                    break;

                case RUNING:
                    // Continuous updates: columns, gravity, animation, collisions, score, etc.
                    col_mov(column);
                    bird_fall(&bird);
                    update_bird_animation(&bird);

                    if (collision(column, &bird)) {
                        // TODO: change to GAME_OVER, subtract life, etc.
                        exit(0);// temporary behavior
                    }
                    break;

                default:
                    break;
            }

        // 4) Render exactly once per frame (background → world → HUD)
        prepareScene(&app);                    // clear with background color
        draw_background(&background, &app);   // draw ground/sky first
        draw_col(column, &app);               // then world (columns)
        draw_bird(&bird, &app);               // then player (and finally HUD if you have it)
        SDL_RenderPresent(app.renderer);      // show the composed frame

        // 5) If you did NOT enable VSYNC above, be nice to the CPU:
        SDL_Delay(16);
        
    }

    free(column);
    cleanupSDL(&app, &bird, column, &background);
    return 0;
}