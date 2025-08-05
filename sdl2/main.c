#include "backend.h"
#include "frontend.h"
//Variables Globales
int GAME_WIDTH;              //Stores the WIDTH of the screen
int GAME_HEIGHT;             //Stores the HEIGHT of the screen
int HOLE_HEIGHT;        //Stores the Height of the holes
int NUM_COL;            //Stores the number of columns including the ones OUTSIDE the screen
int COL_WIDTH;          //Stores the WIDTH of the columns
int SPACE;              //Stores the space between the columns
int BIRD_SCALE=4;       // Stores the scale of the bird from the real size of the img

column_t* column;       //stores all directions of the columns, they are not in order


int main(void){
    bird_t bird; //init bird
    menu_t menu ={.state=MAIN_MENU};
    app_t app;
    init_parameters();
    column = malloc(sizeof(column_t) * NUM_COL);

    memset(&app, 0, sizeof(app));       //inicialices all the values of app in 0
    initSDL(&app);

    init(column, &bird, &menu, &app);

    //game loop
    while (1){
        //game logic
        update_bird_animation(&bird);

        prepareScene(&app);
        window_input();
        display_bird(&bird, &app);
        presentScene(&app);

        SDL_Delay(16);      //16 ms == 62 FPS 
    }

    free(column);
    cleanupSDL(&app);
    return 0;
}