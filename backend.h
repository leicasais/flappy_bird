#ifndef BACKEND_H
#define BACKEND_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <ncurses.h>
    #include <time.h>  

    //Macros
    #define WIDTH 40       // width screen game parameters
    #define HEIGHT 20      // height screen game parameters
    #define COL_WIDTH 3     //width of columns 
    #define SPACE 3         //spaces between col
    #define HOLE_HEIGHT HEIGHT/3 //Height of the hole
    #define NUM_COL WIDTH/(COL_WIDTH*SPACE) ////Num of columns

    //Structs
    typedef struct{     
        int x;      //Saves where are the holes begin in coord x
        int y;      //Saves where are the holes begin in coord y

    }coord_t;

    //Prototipos 
    void init(coord_t* pcol, coord_t* pbird);
    int rand_hole(void);

#endif