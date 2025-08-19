#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int TILE_HIGHT;
extern int HOLE_HEIGHT;           
extern int NUM_COL;
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;


//Column funcions

static float col_scroll_accum = 0.0f;

void col_mov(column_t* pcol){
    // 1) acumulamos velocidad y convertimos a pasos enteros
    col_scroll_accum += pcol->col_speed;             // pcol->col_speed vive en column[0]
    int step = (int)col_scroll_accum;
    if (step <= 0){
        return;
    }
    col_scroll_accum -= step;

    // 2) mover y recortar/crecer la parte visible según corresponda
    for (int i = 0; i < NUM_COL; i++) {
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
            }
        } 
        else if (pcol[i].len < COL_WIDTH) {
            // entrando por la derecha: voy “descubriendo” la textura
            pcol[i].len += step;
            if (pcol[i].len > COL_WIDTH) {
                pcol[i].len = COL_WIDTH;
            }
        }
    }

    // 3) ¿hay lugar a la derecha para spawn de una nueva columna?
    int rightmost_right = 0;                          // borde derecho más lejano
    for (int j = 0; j < NUM_COL; j++) {
        if (pcol[j].len <= 0) {
            continue;
        }
        int r = (int)pcol[j].x + pcol[j].len;
        if (r > rightmost_right) {
            rightmost_right = r;
        }
    }

    // Si ya hay espacio libre (borde derecho + SPACE < borde de ventana), activamos una nueva
    if (rightmost_right + SPACE <= GAME_WIDTH - 1) {
        for (int i = 0; i < NUM_COL; i++) {
            if (pcol[i].len == 0) {
                pcol[i].x   = GAME_WIDTH - 1;        // pegada al borde derecho
                pcol[i].y   = rand_hole();
                pcol[i].len = 1;                     // empieza “chiquita”, entra de a poco
                pcol[i].trim = 0;                    // re-habilitar conteo de score
                break;
            }
        }
    }
}

void col_reset_scroll(void){
    col_scroll_accum = 0.0f;
}


int rand_hole(void){ //returns a random coord y for the begining of the hole  OBS-> min + rand() % (max - min + 1);      
    const int margin_top = 2;  // lo que quieras dejar libre arriba
    int minY = margin_top;
    int maxY = GAME_HEIGHT - TILE_HIGHT - HOLE_HEIGHT - 1; // límite correcto

    if (maxY < minY) {
        // agujero imposible de colocar: ajustá HOLE_HEIGHT o márgenes
        maxY = minY;
    }
    return minY + rand() % (maxY - minY + 1); // rango [minY, maxY]
}
