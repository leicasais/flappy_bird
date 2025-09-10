#include "game_logic.h"
#include "menu.h"
#include "init_fun.h"

char collision(column_t* pcol, bird_t* pbird, screen_dim_t *screen_dim){
    int hair_px = (2*8) / pbird->scale;
    int foot_px= (1*8) / pbird->scale;
    for (int i = 0; i < screen_dim->NUM_COL; i++) {
        if (pcol[i].x == OUTSIDE){
            continue; // skip unactive columns
        }
        int col_left = pcol[i].x;
        int col_right = col_left + pcol[i].len;
        int hole_top = pcol[i].y;
        int hole_bottom = hole_top + screen_dim->HOLE_HEIGHT;

        if(pbird->y_bottom >= screen_dim->GAME_HEIGHT- screen_dim->TILE_HIGHT -1){
            return 1;
        }
        else if ((pbird->x_r >= col_left) && (pbird->x_l <= col_right)) {
            // check if the bird is outside of the hole
            if ((pbird->y_bottom - foot_px) >= hole_bottom || (pbird->y_top + hair_px) <= hole_top) {
                return 1;
            }
        }
    }
    return 0;
}

void points(column_t* pcol, bird_t* pbird, menu_t* menu, screen_dim_t *screen_dim){
    for (int i=0; i < screen_dim->NUM_COL; i++){
        if (pcol[i].len == 0){
            continue;
        } // skips innactive columns
        int col_right = pcol[i].x + pcol[i].len;

        //Adds a point when the bird passes the right corner of the column
        if (!pcol[i].trim && (col_right < (int)pbird->x_l)) {
            pcol[i].trim = 1;
            if(pcol->col_speed >= SPEED_MAX/2){     //Aditional score: passing 1 col = 2 points
                menu->score+=2;
            }
            else{   //normal score: passing 1 col = 1 point
                menu->score++;
            }           

            // inccrements the velocity of the columns each time the bird passes a col (it has a max speed)
            pcol->col_speed += SPEED_INC;
            SDL_Log("Dificult = %d", menu->dificulty);
            SDL_Log("SPEED = %.2f", pcol->col_speed);
            if (pcol->col_speed > SPEED_MAX) {
                pcol->col_speed = SPEED_MAX;
            }
        }

    }
}

void colition_update(menu_t* pmenu){     // Updates game statistics such as score and lives.
    pmenu->lives --;
    if(pmenu->lives < 1){
        menu_set_state(pmenu, GAME_OVER);
        history_log(pmenu);
        pmenu->last_top_pos = score_update(pmenu,pmenu->score);
        score_save(pmenu);
    }
}

void history_log(menu_t* pmenu){
    time_t t;
    struct tm * info;
    time(&t);
    info = localtime(&t);
    FILE *f = fopen("history_log.txt","a");
    fprintf(f,"Username: %s\t Score: %d\t at %s",pmenu->username,pmenu->score,asctime(info));
    fclose(f);
}

void score_init(menu_t *pmenu) {
    // set the values in 0
    for (int i = 0; i < MAX_SCORES; i++){
        pmenu->high_score[i] = 0;
    }
    FILE *f = fopen("scores.txt", "r");
    if (!f) {
        // if it doestn exist, create it in values=0
        score_save(pmenu);
    }
    else{
        for(int j,i=0;i < MAX_SCORES && fscanf(f, "%d", &j) == 1;i++) {
            pmenu->high_score[i] = j;
        }
        fclose(f);
    }
}

int score_update(menu_t *pmenu, int new_score) {
    //find the right position for the final score (descending order)
    int pos = MAX_SCORES;
    for (int i = 0; i < MAX_SCORES && pos == MAX_SCORES; i++){
        if (new_score >= pmenu->high_score[i]){ 
            pos = i; 
        }
    }
    if (pos == MAX_SCORES){
        return 0;
    }
    else{
        for (int i = MAX_SCORES - 1; i > pos; i--){
            pmenu->high_score[i] = pmenu->high_score[i-1];
        }
        pmenu->high_score[pos] = new_score;
        return pos+1;
    }
}

void score_save(menu_t *pmenu) {
    FILE *f = fopen("scores.txt", "w");
    if (!f){
        SDL_Log("score_save: cannot open %s", "scores.txt");
    }
    else{
        for(int i = 0; i < MAX_SCORES; i++){
            fprintf(f, "%d\n", pmenu->high_score[i]);
        }
        fclose(f);
    }
}


void game_reset(column_t* pcol, bird_t *bird, menu_t *menu, screen_dim_t *screen_dim){
    //Create a temporal "save_name" string to no lose de orginial name
    char saved_name[USERNAME_MAX + 1];
    strncpy(saved_name, menu->username, USERNAME_MAX);
    saved_name[USERNAME_MAX] = '\0';
    
    init(pcol,bird, menu, screen_dim);

    strncpy(menu->username, saved_name, USERNAME_MAX);
    menu->username[USERNAME_MAX] = '\0';
    menu->name_editing = 0;

    // *** menú / score ***
    menu->score = 0;
    menu->lives = 3;
    menu->selected = 0;
    menu->last_top_pos = 0;
    
}