#include "backend.h"
#ifndef FRONTEND_H
#define FRONTEND_H

    //Prototipes
    void display_col(column_t* pcol);
    void display_bird(bird_t * bird, int ch);
    void display_main_menu(int selection);
    void display_game_over_menu( int selection);
    void display_resurecting_bird(bird_t * bird, int ch);
    void display_upper_line(int lives);

#endif