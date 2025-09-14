#include "menu.h"
#include "game_logic.h"
#include "init_fun.h"


static int menu_num_options(int state){
    switch (state){
        case MAIN_MENU:  
            return NUM_OPTIONS_MAIN;      // 0: Play, 1: Select Skin, 2: Difficulty, 3: Exit
        case PAUSE:      
            return NUM_OPTIONS_PAUSE;     // 0: Resume, 1: Restart, 2: Menu, 3: Exit
        case GAME_OVER:  
            return NUM_OPTIONS_GAME_OVER; // 0: Retry, 1: Menu
        case SKIN_MENU:
            return NUM_OPTIONS_SKIN;      // 0: 5 Skins
        case DIFICULTY_MENU:
            return NUM_OPTIONS_DIFICULTY; // 0: 4 Difficulties
        default:         
            return 0;
    }
}

void menu_init(menu_t *menu){
    menu->state         = NAME_MENU;
    menu->selected      = 0;
    menu->score         = 0;
    menu->lives         = 3;
    menu->heart_h       = 256/4 ; // px
    menu->heart_w       = 256/4;  // px
    menu->last_top_pos  = 0;
    menu->username[0]   = '\0';        
    menu->name_editing  = 1;      
    menu->dificulty = EASY;     
    score_init(menu); // loads/normalizes high scores
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

void menu_activate_selected(menu_t *menu, column_t *cols, bird_t *bird, app_t *app, screen_dim_t *screen_dim){
    (void)app;

    switch (menu->state) {
        case MAIN_MENU: // 0: Play, 1: Select Skin, 2: Difficulty, 3: Exit
            if (menu->selected == 0){
                game_reset(cols, bird, menu, screen_dim);
                menu_set_state(menu, BEGINING);
            } 
            else if (menu->selected == 1){
                menu->selected = 0;                 // starts selecting the 1st skin
                menu_set_state(menu, SKIN_MENU);
            } 
            else if (menu->selected == 2){
                menu->selected = 0; // starts with the first difficulty
                menu_set_state(menu, DIFICULTY_MENU);
            }
            else {
                menu_set_state(menu, EXIT);
            }
        break;

        case PAUSE:     // 0: Resume, 1: Restart, 2: Menu, 3: Exit
            if (menu->selected == 0){
                menu_set_state(menu, RUNING);
            } 
            else if (menu->selected == 1){
                game_reset(cols, bird, menu, screen_dim);
                menu_set_state(menu, BEGINING);
            } 
            else if (menu->selected == 2){
                menu_set_state(menu, MAIN_MENU);
            } 
            else{
                menu_set_state(menu, EXIT);
            }
        break;

        case GAME_OVER: // 0: Retry, 1: Menu, 2: Exit
            //Save score before change the scene
            if (menu->selected == 0){                 // Play again
                game_reset(cols, bird, menu, screen_dim);
                menu_set_state(menu, BEGINING);
            } 
            else{                                   // Exit
                menu_set_state(menu, EXIT);
            }
        break;

        case SKIN_MENU: // 5 skins
            set_bird_skin(bird, app, menu->selected);  // apply selected skin
            menu->selected = 0;                         // return with the 1st option in Main
            menu_set_state(menu, MAIN_MENU);
        break;

        case DIFICULTY_MENU:
            if(menu->selected==0){
                menu->dificulty=EASY;
            }
            else if(menu->selected==1){
                menu->dificulty=MEDIUM;
            }
            else if(menu->selected==2){
                menu->dificulty=EXTREME;
            }
            else{
                menu->dificulty=IMPOSSIBLE;
            }
            menu->selected=0;
            menu_set_state(menu, MAIN_MENU);
        break;
        
        default:
        break;
    }
}