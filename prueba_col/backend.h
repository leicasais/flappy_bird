#ifndef BACKEND_H
#define BACKEND_H

    /*#########################################

                        MACROS

    #########################################*/
    #define OUTSIDE -1000
    
    #define NUM_OPTIONS_MAIN 2 
    #define MAIN_MENU 1
    #define RUNING 2
    #define PAUSE 3
    #define RESUME 4
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
        int y;
        float vel_y;
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
    void init(column_t* pcol);            // Inicialisation of the var
    int rand_hole(void);                                //Inicialices a random position y for the hole
    char collision(column_t* pcol, bird_t* pbird);      // Returns 1 if the bird's position will collide with a column in the next frame; otherwise, returns 0.
    void col_mov(column_t* pcol);                       //Changes the coord x in each col per frame
    void set_parameters(void);                          //Set default values for the game, like hole size 
    void bird_mov(bird_t *b);              //Update the bird velocity
    void bird_jump(bird_t *b);                          //Scan a "jump"
    void main_menu(int key, menu_t *menu, int *selection);                                // Displays the main menu and returns a code based on the player’s selection (e.g., start, exit).


#endif

//std libs and ncurses
#ifndef EXTERN_LIBS_H
#define EXTERN_LIBS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <ncurses.h>
    #include <signal.h>     //for screen changing size
    #include <time.h>
    #include <unistd.h>  // usleep()
#endif
