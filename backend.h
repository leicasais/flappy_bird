#ifndef BACKEND_H
#define BACKEND_H

    //Structs
    typedef struct{ //Origin in (1,1)    
        int x;      //Saves where are the holes begin in coord x
        int y;      //Saves where are the holes begin in coord y

    }coord_t;

    //Prototipos 
    void init(coord_t* pcol, coord_t* pbird);// INicialisation of the var
    int rand_hole(void);
    int lines_col(coord_t pcol);//Rerturns the number of lines shown in the screen in a Col
    void col_mov(coord_t* pcol);//Changes the coord x in each col per frame
    void handle_winch(int sig);
    int update_screen_dimensions();


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
