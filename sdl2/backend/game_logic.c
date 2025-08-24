#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int TILE_HIGHT;
extern int HOLE_HEIGHT;
extern int COL_BOTTOM_WIDTH;
extern int COL_TOP_HIGH;            
extern int NUM_COL;
extern int COL_WIDTH;
extern int SPACE;



char collision(column_t* pcol, bird_t* pbird){
    int hair_px = (2*8) / pbird->scale;
    int foot_px= (1*8) / pbird->scale;
    for (int i = 0; i < NUM_COL; i++) {
        if (pcol[i].x == OUTSIDE){
            continue; // Saltar columnas no activas
        }
        int col_left = pcol[i].x;
        int col_right = col_left + pcol[i].len;
        int hole_top = pcol[i].y;
        int hole_bottom = hole_top + HOLE_HEIGHT;

        if(pbird->y_bottom >= GAME_HEIGHT-TILE_HIGHT-1){
            return 1;
        }
        else if ((pbird->x_r >= col_left) && (pbird->x_l <= col_right)) {
            // Verificar si el pájaro NO está dentro del hueco
            if ((pbird->y_bottom - foot_px) >= hole_bottom || (pbird->y_top + hair_px) <= hole_top) {
                return 1;
            }
        }
    }
    return 0;
}

void points(column_t* pcol, bird_t* pbird, menu_t* menu){
    for (int i=0; i < NUM_COL; i++){
        if (pcol[i].len == 0){
            continue;
        } // Saltar columnas no activas
        int col_right = pcol[i].x + pcol[i].len;

        // Cuenta la columna cuando el borde derecho quedó a la izquierda del pájaro
        if (!pcol[i].trim && (col_right < (int)pbird->x_l)) {
            pcol[i].trim = 1;           // marcar como contada
            menu->score++;

            // subir velocidad suavemente (con tope)
            pcol->col_speed += SPEED_INC;
            SDL_Log("SPEED = %.2f", pcol->col_speed);
            if (pcol->col_speed > SPEED_MAX) {
                pcol->col_speed = SPEED_MAX;
            }
        }

    }
}

/*void hud_update_score(app_t* app, TTF_Font* font, SDL_Color color,int score, SDL_Texture** out_tex, int* out_w, int* out_h) {
    static int last = -1;
    if (score == last && *out_tex) return;  //canges the number only if there was a change in the score
    last = score;

    if (*out_tex) { SDL_DestroyTexture(*out_tex); *out_tex = NULL; }

    char buf[32]; snprintf(buf, sizeof(buf), "%d", score);
    SDL_Surface* surf = TTF_RenderUTF8_Solid(font, buf, color); // bordes “duros”
    if (!surf) { SDL_Log("TTF_Render FAIL: %s", TTF_GetError()); return; }

    *out_tex = SDL_CreateTextureFromSurface(app->renderer, surf);
    *out_w = surf ->w;
    *out_h = surf->h;
    SDL_FreeSurface(surf);
    SDL_SetTextureBlendMode(*out_tex, SDL_BLENDMODE_BLEND);
}
*/

void colition_update(menu_t* pmenu){     // Updates game statistics such as score and lives.
    pmenu->lives --;
    if(pmenu->lives < 1){
        menu_set_state(pmenu, GAME_OVER);
        pmenu->last_top_pos = score_update(pmenu,pmenu->score);
        score_save(pmenu);
    }
}

void score_init(menu_t *pmenu) {
    // arranco todo en cero por si el archivo no existe o tiene menos líneas
    for (int i = 0; i < MAX_SCORES; i++){
        pmenu->high_score[i] = 0;
    }
    FILE *f = fopen("scores.txt", "r");
    if (!f) {
        // si no existe, lo creo con ceros y salgo
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
    // encontrar posición de inserción (orden descendente)
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
        // desplazar hacia abajo y colocar
        for (int i = MAX_SCORES - 1; i > pos; i--){
            pmenu->high_score[i] = pmenu->high_score[i-1];
        }
        pmenu->high_score[pos] = new_score;
        return pos+1;
    }
}

void score_save(menu_t *pmenu) {
    //scores
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


void game_reset(column_t* pcol, bird_t *bird, menu_t *menu){
    init(pcol,bird, menu);
    // *** menú / score ***
    menu->score = 0;
    menu->lives = 3;
    menu->selected = 0;
    menu->last_top_pos = 0;
    
}