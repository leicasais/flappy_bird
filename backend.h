#ifndef BACKEND_H
#define BACKEND_H

    //MACROS
    #define OUTSIDE -1000

    //Structs
    typedef struct{ //Origin in (1,1)    
        int x;      //Saves where are the holes begin in coord x
        int y;      //Saves where are the holes begin in coord y
        int len; //Saves the length of the column

    }column_t;
    typedef struct{
        int y;
        float vel_y;
    }bird_t;

    //Prototipos 
    void init(column_t* pcol);// INicialisation of the var
    int rand_hole(void);
    int lines_col(column_t pcol);//Rerturns the number of lines shown in the screen in a Col
    void col_mov(column_t* pcol);//Changes the coord x in each col per frame
    void handle_winch(int sig);
    int update_screen_dimensions();
    void set_parameters(void); //Set default values for the game, like hole size 
    void bird_init(bird_t *bird); //Inicialize the position of the bird
    void bird_update(bird_t *b, float dt); //Update the bird velocity
    void bird_jump(bird_t *b); //Scan a "jump"


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
