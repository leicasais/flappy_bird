#include "backend.h"


static int menu_num_options(int state){
    switch (state){
        case MAIN_MENU:  
            return NUM_OPTIONS_MAIN;      // 0: Jugar, 1: Salir
        case PAUSE:      
            return NUM_OPTIONS_PAUSE;     // 0: Continuar, 1: Reiniciar, 2: Menu, 3: Salir
        case GAME_OVER:  
            return NUM_OPTIONS_GAME_OVER; // 0: Reintentar, 1: Menus
        default:         
            return 0;
    }
}

void menu_init(menu_t *menu){
    menu->state         = MAIN_MENU;
    menu->selected      = 0;
    menu->score         = 0;
    menu->lives         = 3;
    menu->last_top_pos  = 0;
    score_init(menu); // carga/normaliza highscores
}

void menu_set_state(menu_t *menu, int state){
    menu->state = state;
    menu->selected = 0;
}

void menu_next_option(menu_t *menu){
    int n = menu_num_options(menu->state);
    if (n <= 0){ 
        return;
    }
    menu->selected = (menu->selected + 1) % n;
}

void menu_prev_option(menu_t *menu){
    int n = menu_num_options(menu->state);
    if (n <= 0){
        return;
    }
    menu->selected = (menu->selected - 1 + n) % n;
}

void menu_activate_selected(menu_t *menu, column_t *cols, bird_t *bird, app_t *app){
    (void)app;

    switch (menu->state) {
        case MAIN_MENU: // 0: Jugar, 1: Salir
            if (menu->selected == 0){
                game_reset(cols, bird, menu);
                menu_set_state(menu, RUNING);
                menu->state=BEGINING;       //sets the game in pause until player presses space
            } 
            else{
                menu_set_state(menu, EXIT);
            }
            break;

        case PAUSE:     // 0: Continuar, 1: Reiniciar, 2: Menu, 3: Salir
            if (menu->selected == 0){
                menu_set_state(menu, RUNING);
            } 
            else if (menu->selected == 1){
                game_reset(cols, bird, menu);
                menu_set_state(menu, RUNING);
            } 
            else if (menu->selected == 2){
                menu_set_state(menu, MAIN_MENU);
            } 
            else{
                menu_set_state(menu, EXIT);
            }
            break;

        case GAME_OVER: // 0: Reintentar, 1: Menu, 2: Salir
            // guardo score ANTES de cambiar de escena
            if (menu->selected == 0) {                 // Volver a jugar
                game_reset(cols, bird, menu);
                menu_set_state(menu, BEGINING);
            } 
            else {                                   // Salir
                menu_set_state(menu, EXIT);
            }
            break;

        default:
            break;
    }
}
