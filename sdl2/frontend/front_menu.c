#include "frontend.h"
#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int HOLE_HEIGHT;
extern int TILE_HIGHT;
extern int NUM_COL;
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;


void render_main_menu(app_t *app, menu_t *menu, int w, int h){
    // 1) dibujamos la tarjeta
    const int cardW = (int)(w * PANEL_W_RATIO);
    const int cardH = (int)(h * PANEL_H_RATIO);
    const int cardX = (w - cardW) / 2;
    const int cardY = (h - cardH) / 2;

    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(app->renderer, 30, 30, 30, 200);
    SDL_Rect card = { cardX, cardY, cardW, cardH };
    SDL_RenderFillRect(app->renderer, &card);
    SDL_SetRenderDrawColor(app->renderer, 220, 220, 220, 255);
    SDL_RenderDrawRect(app->renderer, &card);

    // 2) título y subtítulo
    int tw=0, th=0;
    SDL_Color titleCol = (SDL_Color){230, 230, 80, 255};
    SDL_Color textCol  = (SDL_Color){240, 240, 240, 255};

    draw_text_center(app->renderer, app->font, "FLAPPY", titleCol, cardX + cardW/2, cardY + 22, &tw, &th);

    draw_text_center(app->renderer, app->font, "Presiona Enter para comenzar", textCol, cardX + cardW/2, cardY + 22 + th + 10, NULL, NULL);

    // 3) opciones
    const char *opts[] = { "Play", "Salir" };
    const int n = 2;
    const int lineH = ((th) ? (th) + 16 : 40);
    int startY = cardY + cardH/2 - (n * lineH)/2;

    for (int i = 0; i < n; ++i) {
        int y = startY + i * lineH;
        if (i == menu->selected) {
            // highlight de opción seleccionada
            SDL_SetRenderDrawColor(app->renderer, 70, 120, 200, 180);
            SDL_Rect hi = { cardX + 20, y - 6, cardW - 40, lineH };
            SDL_RenderFillRect(app->renderer, &hi);
        }
        draw_text_center(app->renderer, app->font, opts[i], textCol, cardX + cardW/2, y, NULL, NULL);
    }
}

// ===== tarjeta y lista del menú =====
void draw_menu_list(SDL_Renderer *r, int w, int h, const char **options, int n, int selected, const char *title, const char *subtitle)
{
    // tarjeta centrada
    const int cardW = w * 0.45;
    const int cardH = h * 0.50;
    const int cardX = (w - cardW) / 2;
    const int cardY = (h - cardH) / 2;

    // fondo tarjeta
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 30, 30, 30, 200);
    SDL_Rect card = { cardX, cardY, cardW, cardH };
    SDL_RenderFillRect(r, &card);
    SDL_SetRenderDrawColor(r, 220, 220, 220, 255);
    SDL_RenderDrawRect(r, &card);

    extern app_t app; // si no tenés un app global, pasá font/color por parámetro
    // Alternativa segura: reemplazá 'app.font' por el font que tengas a mano.
}

void render_pause_menu(app_t *app, menu_t *menu, int w, int h)
{
    // tarjeta
    const int cardW = (int)(w * PANEL_W_RATIO);
    const int cardH = (int)(h * PANEL_H_RATIO);
    const int cardX = (w - cardW) / 2;
    const int cardY = (h - cardH) / 2;

    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(app->renderer, 30, 30, 30, 200);
    SDL_Rect card = { cardX, cardY, cardW, cardH };
    SDL_RenderFillRect(app->renderer, &card);
    SDL_SetRenderDrawColor(app->renderer, 220, 220, 220, 255);
    SDL_RenderDrawRect(app->renderer, &card);

    // título y subtítulo
    SDL_Color titleCol = (SDL_Color){230, 230, 80, 255};
    SDL_Color textCol  = (SDL_Color){240, 240, 240, 255};
    int tw=0, th=0;

    draw_text_center(app->renderer, app->font, "PAUSA", titleCol, cardX + cardW/2, cardY + 22, &tw, &th);
    draw_text_center(app->renderer, app->font, "Esc para volver", textCol, cardX + cardW/2, cardY + 22 + th + 8, NULL, NULL);

    // opciones
    const char *opts[] = { "Reanudar", "Reiniciar", "Volver al main menu", "Salir" };
    const int n = 4;
    const int lineH = OPTION_LINE_H;
    const int gap   = OPTION_GAP;

    // bloque de opciones al pie 
    const int optsH = n * lineH + (n - 1) * gap;
    const int optYStart = cardY + cardH - PANEL_BOTTOM_PAD - optsH;

    for (int i = 0; i < n; ++i) {
        const int yy = optYStart + i * (lineH + gap);
        if (i == menu->selected) {
            SDL_SetRenderDrawColor(app->renderer, 70, 120, 200, 180);
            SDL_Rect hi = { cardX + HILIGHT_INSET, yy - OPTION_HILIGHT_PAD_Y, cardW - 2*HILIGHT_INSET, OPTION_LINE_H + 2*OPTION_HILIGHT_PAD_Y };
            SDL_RenderFillRect(app->renderer, &hi);
        }
        draw_text_center(app->renderer, app->font, opts[i], textCol, cardX + cardW/2, yy, NULL, NULL);
    }
}




// Muestra: "GAME OVER", subtítulo con Score, y opciones: Reintentar / Salir
void render_game_over(app_t *app, menu_t *menu, int w, int h){
    // tarjeta
    const int cardW = (int)(w * PANEL_W_RATIO);
    const int cardH = (int)(h * PANEL_H_RATIO);
    const int cardX = (w - cardW) / 2;
    const int cardY = (h - cardH) / 2;

    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(app->renderer, 30, 30, 30, 200);
    SDL_Rect card = { cardX, cardY, cardW, cardH };
    SDL_RenderFillRect(app->renderer, &card);
    SDL_SetRenderDrawColor(app->renderer, 220, 220, 220, 255);
    SDL_RenderDrawRect(app->renderer, &card);

    // título + puntaje
    int tw=0, th=0;
    SDL_Color titleCol = (SDL_Color){230,230,80,255};
    SDL_Color textCol  = (SDL_Color){240,240,240,255};

    draw_text_center(app->renderer, app->font, "GAME OVER", titleCol, cardX + cardW/2, cardY + 22, &tw, &th);

    char sub[64];
    snprintf(sub, sizeof(sub), "Puntaje: %d", menu->score);
    draw_text_center(app->renderer, app->font, sub, textCol, cardX + cardW/2, cardY + 22 + th + 8, NULL, NULL);

    int y = cardY + 22 + th + 8;
    // mensaje de felicitación si entró al Top
    if (menu->last_top_pos > 0) {
        char congr[96];
        snprintf(congr, sizeof(congr), "Felicitaciones, entraste al top %d!", menu->last_top_pos);
        y += 22;
        draw_text_center(app->renderer, app->font, congr, textCol, cardX + cardW/2, y, NULL, NULL);
    }

    // Título de la tabla
    y += 28;
    draw_text_center(app->renderer, app->font, "TOP 10", titleCol, cardX + cardW/2, y, NULL, NULL);


    // --- LISTA TOP-10 ---
    y += 20;                            // un poquito menos de gap
    const int listX = cardX + 40;      // margen izquierdo de la tabla

    // Primero calculamos dónde empiezan las opciones para saber
    // cuánto alto nos queda para la lista
    const char* opts[] = { "Volver a jugar", "Salir" };
    const int n = 2;
    const int lineH = OPTION_LINE_H;
    const int gap   = OPTION_GAP;
    const int optsH = n * lineH + (n - 1) * gap;
    const int optYStart = cardY + cardH - PANEL_BOTTOM_PAD - optsH;

    // Altura disponible para 10 filas
    int listStartY = y;
    int availH = optYStart - listStartY - LIST_OPTIONS_GAP;        // 6 px de respiro
    if (availH < 10) {
        availH = 10;
    }

    // Altura base de la fuente y escala para que entren las 10 filas
    int baseH  = TTF_FontHeight(app->font);
    int rowH   = availH / MAX_SCORES;               // alto de cada renglón
    float scale = (float)rowH / (float)baseH;       // escala de texto para ese alto
    if (scale > 0.95f) {
        scale = 0.95f;               // no agrandes
    }
    if (scale < 0.65f) {
        scale = 0.65f;               // no achiques demasiado
    }
    // Ajusto rowH en función de la escala real y dejo 2 px de padding interno
    int text_h = (int)(baseH * scale);
    rowH = text_h + 4;

    for (int i = 0; i < MAX_SCORES; ++i) {
        const int rowY = listStartY + i * rowH;

        // Subrayado del renglón completo si el score nuevo entró en esa posición
        if (menu->last_top_pos == i + 1) {
            SDL_SetRenderDrawColor(app->renderer, 70, 120, 200, 140);
            SDL_Rect hi = { cardX + 20, rowY - 2, cardW - 40, rowH };
            SDL_RenderFillRect(app->renderer, &hi);
        }

        char line[48];
        snprintf(line, sizeof(line), "%2d) %d", i + 1, menu->high_score[i]);

        // Dibujamos el texto centrado *verticalmente* dentro de la fila
        int tw=0, th=0;
        draw_text_left_scaled(app->renderer, app->font, line, textCol, listX, rowY + (rowH - text_h)/2, scale, &tw, &th);
    }

    // --- Opciones al pie ---
    for (int i = 0; i < n; ++i) {
        const int yy = optYStart + i * (lineH + gap);
        if (i == menu->selected) {
            SDL_SetRenderDrawColor(app->renderer, 70, 120, 200, 180);
            SDL_Rect hi = {cardX + HILIGHT_INSET, yy - OPTION_HILIGHT_PAD_Y, cardW - 2*HILIGHT_INSET, OPTION_LINE_H + 2*OPTION_HILIGHT_PAD_Y};
            SDL_RenderFillRect(app->renderer, &hi);
        }
        draw_text_center(app->renderer, app->font, opts[i], (SDL_Color){240,240,240,255}, cardX + cardW/2, yy, NULL, NULL);
    }
}