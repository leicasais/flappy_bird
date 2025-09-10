#ifndef MENU_H
#define MENU_H

    //Incluide Typedefs and Macros
    #include "structs.h"
    
    //menu.c
    void menu_init(menu_t *menu);
    void menu_set_state(menu_t *menu, int state);
    void menu_next_option(menu_t *menu);
    void menu_prev_option(menu_t *menu);
    void menu_activate_selected(menu_t *menu, column_t *cols, bird_t *bird, app_t *app, screen_dim_t *screen_dim);
    
#endif