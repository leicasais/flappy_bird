#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int HOLE_HEIGHT;
extern int COL_BOTTOM_WIDTH;
extern int COL_TOP_HIGH;            
extern int NUM_COL;
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;



char collision(column_t* pcol, bird_t* pbird){
    for (int i = 0; i < NUM_COL; i++) {
        if (pcol[i].x == OUTSIDE){
            continue; // Saltar columnas no activas
        }

        int col_left = pcol[i].x;
        int col_right = col_left + pcol[i].len;
        int hole_top = pcol[i].y;
        int hole_bottom = hole_top + HOLE_HEIGHT;

        if ((pbird->x_r >= col_left) && (pbird->x_l <= col_right)) {
            // Verificar si el pájaro NO está dentro del hueco
            if (pbird->y_bottom >= hole_bottom || pbird->y_top <= hole_top) {
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
    if(pmenu->lives < 0){
        pmenu->state = GAME_OVER;
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
    // *** columnas: igual que en init(), pero sin crear texturas ***
    int aux_x = 0; 
    int i;
    for(i = 0; i < ((GAME_WIDTH/(COL_WIDTH+SPACE)) - 1); i++){
        aux_x += SPACE + COL_WIDTH;  
        pcol[i].x = aux_x;
        pcol[i].y = rand_hole();
        pcol[i].len = COL_WIDTH;
        pcol[i].trim = 0;
    }
    if (GAME_WIDTH % (COL_WIDTH+SPACE)) {
        int space_left = GAME_WIDTH - (COL_WIDTH + SPACE) * (i + 1);
        pcol[i].x = aux_x + SPACE + COL_WIDTH;
        pcol[i].y = rand_hole();
        pcol[i].len = (space_left < COL_WIDTH) ? space_left : COL_WIDTH;
        pcol[i].trim = 0;
        i++;
    }
    for (int j = i; j < NUM_COL; j++){
        pcol[j].x = OUTSIDE;
        pcol[j].len = 0;
        pcol[j].trim = 0;
    }
    pcol->col_speed = BASE_SPEED;   // usar tu “global” guardada en column[0]
    col_reset_scroll();

    // *** pájaro: igual que en init(), pero sin crear textura ***
    bird->x_l    = SPACE/2 + ((HITBOX_X * bird->scale) / 2);
    bird->x_r = bird->x_l + (HITBOX_X * bird->scale);

    float y = 3 + (rand() % (GAME_HEIGHT - (3 + ((HITBOX_Y * bird->scale) / 2))));
    bird->gravity_y = 0.4f;
    bird->vel_y = 0.0f;
    bird->y_top = (int)y;
    bird->y_bottom = bird->y_top + (HITBOX_Y * bird->scale);
    bird->current_frame = 0;
    bird->last_frame_time = SDL_GetTicks();

    // *** menú / score ***
    menu->score = 0;
    menu->lives = 3;
    menu->selected = 0;
    menu->last_top_pos = 0;
}