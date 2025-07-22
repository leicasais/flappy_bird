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

    #define OUTSIDE -1000
    
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



    /*#########################################
                        Structs
    #########################################*/

    typedef struct{ //Origin in (1,1)    
        int x;      //Saves where are the holes begin in coord x
        int y;      //Saves where are the holes begin in coord y
        int len;    //Saves the length of the column

    }column_t;

    typedef struct{
        float y;
        int y_top;               //Creates a hitbox for the bird with top and y_bottom
        int y_bottom;
        float gravity_y;         //based on the terminal velocity of a small bird on each place, delta x per frame that the bird has to descend cause of the gravity 
        float delta_y;              //desplazamiento in y
        int x;
    }bird_t;

    typedef struct {    // menu_t stores game-related data like score, remaining lives, high score, and menu state.
        int score;
        int lives;
        int high_score;
        int state;  // Could represent current menu selection or game state
    } menu_t;



    /*#########################################
                        Functions
    #########################################*/
    
    void init(column_t* pcol, bird_t *bird, menu_t *menu);             // Inicialisation of the var
    int rand_hole(void);                                //Inicialices a random position y for the hole
    char collision(column_t* pcol, bird_t* pbird);      // Returns 1 if the bird's position will collide with a column in the next frame; otherwise, returns 0.
    void col_mov(column_t* pcol);                       //Changes the coord x in each col per frame
    void set_parameters(void);                          //Set default values for the game, like hole size 
    void bird_mov(bird_t* bird, int ch);              //Update the bird velocity
    void main_menu(int key, menu_t *menu, int *selection);                                // Displays the main menu and returns a code based on the player’s selection (e.g., start, exit).
    void game_over_menu(int key, menu_t *menu, int *selection);
    void colition_update(menu_t* pmenu);
    void pause_menu(int key, menu_t *menu, int *selection);
    void points(column_t* pcol, bird_t* pbird, menu_t* menu);

#endif

//std libs and ncurses
#ifndef EXTERN_LIBS_H
#define EXTERN_LIBS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ncurses.h>
    #include <signal.h>     //for screen changing size
    #include <time.h>
    #include <unistd.h>  // usleep()
#endif
