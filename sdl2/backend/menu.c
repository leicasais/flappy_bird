// menu.c
#include "backend.h"

// ayuda interna: cuántas opciones tiene el menú actual
static int menu_num_options(int state) {
    switch (state) {
        case MAIN_MENU:  
            return NUM_OPTIONS_MAIN;     // 0: Play   1: Exit
        case PAUSE:      
            return NUM_OPTIONS_PAUSE;    // 0: Continue 1: Restart 2: Main Menu 3: Exit
        case GAME_OVER:  
            return NUM_OPTIONS_GAME_OVER;// 0: Restart  1: Main Menu 2: Exit
        default:         
            return 0;                    // RUNING u otros: sin opciones
    }
}

void menu_init(menu_t *menu){
    menu->state = MAIN_MENU;
    menu->selected = 0;
    menu->score = 0;
    menu->lives = 3;
    score_init(menu); // carga highscores desde scores.txt (si existe)
}

void menu_set_state(menu_t *menu, int state){
    menu->state = state;
    menu->selected = 0; // al entrar a un menú, reseteo la selección
}

void menu_next_option(menu_t *menu){
    int n = menu_num_options(menu->state);
    if (n <= 0) return;
    menu->selected = (menu->selected + 1) % n;
}

void menu_prev_option(menu_t *menu){
    int n = menu_num_options(menu->state);
    if (n <= 0) return;
    menu->selected = (menu->selected - 1 + n) % n;
}

// Acciona según la opción seleccionada en el menú activo
void menu_activate_selected(menu_t *menu, column_t *cols, bird_t *bird, app_t *app){
    (void)app; // por ahora no lo usamos acá

    switch (menu->state) {

        case MAIN_MENU:
            // 0: Play, 1: Exit
            if (menu->selected == 0) {
                game_reset(cols, bird, menu);
                menu_set_state(menu, RUNING);
            } else {
                menu_set_state(menu, EXIT);
            }
            break;

        case PAUSE:
            // 0: Continue, 1: Restart, 2: Main Menu, 3: Exit
            if (menu->selected == 0) {               // Continue
                menu_set_state(menu, RUNING);
            } else if (menu->selected == 1) {        // Restart
                game_reset(cols, bird, menu);
                menu_set_state(menu, RUNING);
            } else if (menu->selected == 2) {        // Main Menu
                menu_set_state(menu, MAIN_MENU);
            } else {                                  // Exit
                menu_set_state(menu, EXIT);
            }
            break;

        case GAME_OVER:
            // Antes de salir / reiniciar, actualizo y guardo highscores
            score_update(menu, menu->score);
            score_save(menu);

            // 0: Restart, 1: Main Menu, 2: Exit
            if (menu->selected == 0) {
                game_reset(cols, bird, menu);
                menu_set_state(menu, RUNING);
            } else if (menu->selected == 1) {
                menu_set_state(menu, MAIN_MENU);
            } else {
                menu_set_state(menu, EXIT);
            }
            break;

        default:
            // En RUNING u otro estado sin menú, no hay "activate".
            break;
    }
}
