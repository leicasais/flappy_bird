//std libs and SDL2
#ifndef EXTERN_LIBS_H
#define EXTERN_LIBS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <signal.h>     //for screen changing size
    #include <time.h>
    #include <unistd.h>  // usleep()
#endif

#ifndef BACKEND_H
#define BACKEND_H

    /*#########################################
                        MACROS
    #########################################*/

    //Macros for the main/game flow
    #define MS_BTW_FRAMES 16
    #define VEL_COL 4       //numb of frames until the next mov of col
    #define MIN_GAME_HEIGHT 8
    #define MIN_GAME_WIDTH 8 

    //macros for the columns
    #define OUTSIDE -1000

    //macros birds
    #define HITBOX_X 15
    #define HITBOX_Y 16
    
    //macros for the menus
    #define NUM_OPTIONS_MAIN 2 
    #define NUM_OPTIONS_GAME_OVER 3
    #define NUM_OPTIONS_PAUSE 4
    #define MAIN_MENU 1
    #define RUNING 2
    #define PAUSE 3
    #define CONTINUE 4
    #define EXIT 5
    #define RESTART 6
    #define GAME_OVER 7
    #define MAX_SCORES 10



    /*#########################################
                        Structs
    #########################################*/

    typedef struct{
        SDL_Renderer *renderer;
        SDL_Window *window;
    } app_t;
    

    typedef struct{ //Origin in (1,1)    
        int x;      //Saves where are the holes begin in coord x
        int y;      //Saves where are the holes begin in coord y
        int len;    //Saves the length of the column
        SDL_Texture *texture_col_top;      //pointer to img of the top of the column/pipe
        SDL_Texture *texture_col_bottom;   // idem but the bottom
        int trim;                          //cut of the bottom_col
        int current_frame;
        Uint32 last_frame_time;

    }column_t;

    typedef struct{
        int y_top;               //Creates a hitbox for the bird with top and y_bottom
        int y_bottom;
        int x_top;
        int x_bottom;
        float gravity_y;         //based on the terminal velocity of a small bird on each place, delta x per frame that the bird has to descend cause of the gravity 
        float vel_y;
        SDL_Texture *texture;       //saves the bird skin
        int current_frame;
        Uint32 last_frame_time;
    }bird_t;

    typedef struct {    // menu_t stores game-related data like score, remaining lives, high score, and menu state.
        int score;
        int lives;
        int high_score[MAX_SCORES];
        int state;  // Could represent current menu selection or game state
    } menu_t;



    /*#########################################
                        Functions
    #########################################*/
    
    //init_fun.c
    void init_parameters(void);
    void init(column_t* pcol, bird_t *bird, menu_t *menu, app_t *app);             // Inicialisation of the var
    void initSDL(app_t *app);
    void set_parameters(void);                          //Set default values for the game, like hole size 
    void cleanupSDL(app_t *app, bird_t *bird, column_t *column);
    SDL_Texture *loadTexture(char *filename, app_t *app);           //takes a file path (e.g., "bird.png") and returns an SDL_Texture * that you can later render to the screen


    //column_fun.c
    int rand_hole(void);                                //Inicialices a random position y for the hole
    void col_mov(column_t* pcol);                       //Changes the coord x in each col per frame
  
    //bird_fun.c
    void update_bird_animation(bird_t *bird);
    void bird_jump(bird_t* bird);
    void bird_fall(bird_t* bird);              //Update the bird velocity

    //game_logic.c
    char collision(column_t* pcol, bird_t* pbird);      // Returns 1 if the bird's position will collide with a column in the next frame; otherwise, returns 0.
    void colition_update(menu_t* pmenu);
    void points(column_t* pcol, bird_t* pbird, menu_t* menu);

    //input.c
    void window_input(void);

#endif

