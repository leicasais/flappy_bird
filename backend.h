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
        int x;
        int y;
    }bird_t;

    //Prototipos 
    void init(column_t* pcol, bird_t* pbird);// INicialisation of the var
    int rand_hole(void);//Creates a random position y for the holes
    void col_mov(column_t* pcol);//Changes the coord x in each col per frame
    int update_screen_dimensions();
    void set_parameters(void); //Set default values for the game, like hole size 
    char collision(column_t* pcol, bird_t* pbird); // Returns 1 if the bird's position will collide with a column in the next frame; otherwise, returns 0.


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
