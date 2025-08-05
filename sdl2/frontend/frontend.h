#include "backend.h"
#ifndef FRONTEND_H
#define FRONTEND_H

    //Prototipes

    //windows.c
    void prepareScene(app_t *app);
    

    void display_col(column_t* pcol);
    void display_bird(bird_t *bird, app_t *app);
    void display_main_menu(int selection);
    void display_pause_menu(int selection);
    void display_game_over_menu( int selection);
    void display_resurecting_bird(bird_t * bird, int ch);
    void display_upper_line(menu_t menu);

#endif