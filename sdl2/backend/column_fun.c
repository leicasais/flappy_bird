#include "backend.h"

//Column funcions

static float col_scroll_accum = 0.0f;

void col_mov(column_t* pcol, screen_dim_t *screen_dim, menu_t *pmenu){
    // 1) acumulamos velocidad y convertimos a pasos enteros
    col_scroll_accum += pcol->col_speed;             // pcol->col_speed vive en column[0]
    int step = (int)col_scroll_accum;
    if (step <= 0){
        return;
    }
    col_scroll_accum -= step;

    // 2) mover y recortar/crecer la parte visible según corresponda
    for (int i = 0; i < screen_dim->NUM_COL; i++) {
        if (pcol[i].len <= 0){
            continue;
        }              // inactiva

        // mover a la izquierda
        int xi = (int)pcol[i].x - step;
        pcol[i].x = xi;

        if (xi < 0) {
            // salió por la izquierda: recorto lo que se “fue”
            int over = -xi;                           // cuánto se pasó del borde
            pcol[i].x   = 0;
            pcol[i].len -= over;                      // recorto la parte visible
            if (pcol[i].len <= 0) {
                pcol[i].len = 0;
                pcol[i].x   = OUTSIDE;               // desactivar
                pcol[i].col_speed_y = 0;
            }
        } 
        else if (pcol[i].len < screen_dim->COL_WIDTH) {
            // entrando por la derecha: voy “descubriendo” la textura
            pcol[i].len += step;
            if (pcol[i].len > screen_dim->COL_WIDTH) {
                pcol[i].len = screen_dim->COL_WIDTH;
            }
        }
    }

    // 3) ¿hay lugar a la derecha para spawn de una nueva columna?
    int rightmost_right = 0;                          // borde derecho más lejano
    for (int j = 0; j < screen_dim->NUM_COL; j++) {
        if (pcol[j].len <= 0) {
            continue;
        }
        int r = (int)pcol[j].x + pcol[j].len;
        if (r > rightmost_right) {
            rightmost_right = r;
        }
    }

    // Si ya hay espacio libre (borde derecho + SPACE < borde de ventana), activamos una nueva
    if (rightmost_right + screen_dim->SPACE <= screen_dim->GAME_WIDTH - 1) {
        for (int i = 0; i < screen_dim->NUM_COL; i++) {
            if (pcol[i].len == 0) {
                pcol[i].x   = screen_dim->GAME_WIDTH - 1;        // pegada al borde derecho
                pcol[i].y   = rand_hole(screen_dim);
                pcol[i].len = 1;                     // empieza “chiquita”, entra de a poco
                pcol[i].trim = 0;  
                int n = pmenu->dificulty;                 // re-habilitar conteo de score
                if ((rand() % 100) < n) {
                    float vel = 0.4f + (float)(rand() % 61) / 100.0f; // 
                    pcol[i].col_speed_y = (rand()%2) ? vel : -vel;
                }
                break;
            }
        }

    }

    //Part for the "Dinamic Columns"
    int minY = 2;
    const int maxY = screen_dim->GAME_HEIGHT - screen_dim->TILE_HIGHT - screen_dim->HOLE_HEIGHT - 1;
    for (int i = 0; i < screen_dim->NUM_COL; i++){
        if (pcol[i].len <= 0 || pcol[i].x == OUTSIDE){
            continue;
        }
        float speed_y = pcol[i].col_speed_y;
        if (speed_y != 0.0f) {
            pcol[i].y += speed_y;
            // rebote en límites válidos del hueco
            if (pcol[i].y < (float)minY) {
                pcol[i].y = (float)minY;
                pcol[i].col_speed_y = -speed_y;
            } else if (pcol[i].y > (float)maxY) {
                pcol[i].y = (float)maxY;
                pcol[i].col_speed_y = -speed_y;
            }
        }
    }

}

void col_reset_scroll(void){
    col_scroll_accum = 0.0f;
}


int rand_hole(screen_dim_t *screen_dim){ //returns a random coord y for the begining of the hole  OBS-> min + rand() % (max - min + 1);      
    const int margin_top = 2;  // lo que quieras dejar libre arriba
    int minY = margin_top;
    int maxY = screen_dim->GAME_HEIGHT - screen_dim->TILE_HIGHT - screen_dim->HOLE_HEIGHT - 1; // límite correcto

    if (maxY < minY) {
        // agujero imposible de colocar: ajustá HOLE_HEIGHT o márgenes
        maxY = minY;
    }
    return minY + rand() % (maxY - minY + 1); // rango [minY, maxY]
}
