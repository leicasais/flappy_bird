#ifndef BACKEND_H
#define BACKEND_H

    //MACROS
    #define OUTSIDE -1000

    //Structs
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

    //Prototipos 
    void init(column_t* pcol, bird_t *bird);             // INicialisation of the var
    int rand_hole(void);                                //Inicialices a random position y for the hole
    char collision(column_t* pcol, bird_t* pbird);      // Returns 1 if the bird's position will collide with a column in the next frame; otherwise, returns 0.
    void col_mov(column_t* pcol);                       //Changes the coord x in each col per frame
    void set_parameters(void);                          //Set default values for the game, like hole size 
    void bird_mov(bird_t *b);              //Update the bird velocity
    void bird_jump(bird_t *b);                          //Scan a "jump"


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
