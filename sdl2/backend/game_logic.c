#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int HOLE_HEIGHT;
extern int NUM_COL;
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;

/*
char collision(column_t* pcol, bird_t* pbird){
    for (int i = 0; i < NUM_COL; i++) {
        if (pcol[i].len == 0) continue; // Saltar columnas no activas

        int col_left = pcol[i].x;
        int col_right = col_left + pcol[i].len;
        int hole_top = pcol[i].y;
        int hole_bottom = hole_top + HOLE_HEIGHT;

        if ((pbird->x >= col_left) && (pbird->x < col_right)) {
            // Verificar si el pájaro NO está dentro del hueco
            if (pbird->y_bottom <= hole_top || pbird->y_top >= hole_bottom) {
                return 1;
            }
        }
    }
    return 0;
}

void points(column_t* pcol, bird_t* pbird, menu_t* menu){
    for (int i=0; i < NUM_COL; i++){
        if (pcol[i].len == 0) continue; // Saltar columnas no activas
        int col_right = pcol[i].x + pcol[i].len;

        if (pbird->x == col_right+1){
           menu->score++;
        }
    }
}

void colition_update(menu_t* pmenu){     // Updates game statistics such as score and lives.
    pmenu->lives --;
    if(pmenu->lives < 0){
        pmenu->state = GAME_OVER;
    }
}
*/

void score_init(menu_t * pmenu){
    FILE * f;
    f=fopen("scores.txt","r");
    if(f==NULL){
        printf("Error");
    }
    else{
        for(int i=0;i<MAX_SCORES && (fscanf(f, "%d",&pmenu->high_score[i]) == 1);i++);
        fclose(f);
    }
}

void score_update(menu_t * pmenu, int new_score){
    int cont=0;
    for (int i=0; i<MAX_SCORES && cont==0; i++) {
        if (new_score >= pmenu->high_score[i]) {
        cont = i;
    }
}
    for(int i=MAX_SCORES-1; i>cont;i--){
        pmenu->high_score[i]=pmenu->high_score[i-1];
    }
    pmenu->high_score[cont]=new_score;
}   

void score_save(menu_t *pmenu){
    FILE *f = fopen("scores.txt", "w");
    if (f == NULL) {
        printf("error");
    }
    else{
        for(int i=0; i<MAX_SCORES; i++) {
            fprintf(f,"%d\n",pmenu->high_score[i]);
        }
        fclose(f);
    }
}