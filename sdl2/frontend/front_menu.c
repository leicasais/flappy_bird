#include "frontend.h"
#include "backend.h"

SDL_Color color_title(void){
    SDL_Color titleCol = (SDL_Color){217, 215, 161, 255};
    return titleCol;
}
void color_selection(app_t *app){
    SDL_SetRenderDrawColor(app->renderer, 107, 119, 142, 180);
}


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
    SDL_Color titleCol= color_title();
    SDL_Color textCol  = (SDL_Color){240, 240, 240, 255};

    draw_text_center(app->renderer, app->font, "FLAPPY", titleCol, cardX + cardW/2, cardY + 22, &tw, &th);

    draw_text_center(app->renderer, app->font, "Press ENTER to start", textCol, cardX + cardW/2, cardY + 22 + th + 10, NULL, NULL);

    // 3) opciones
    const char *opts[] = { "Play", "Select Skin" , "Exit game :(" };
    const int n = 3;
    const int lineH = ((th) ? (th) + 16 : 40);
    int startY = cardY + cardH/2 - (n * lineH)/2;

    for (int i = 0; i < n; ++i) {
        int y = startY + i * lineH;
        if (i == menu->selected) {
            // highlight de opción seleccionada
            color_selection(app);
            SDL_Rect hi = { cardX + 20, y - 6, cardW - 40, lineH };
            SDL_RenderFillRect(app->renderer, &hi);
        }
        draw_text_center(app->renderer, app->font, opts[i], textCol, cardX + cardW/2, y, NULL, NULL);
    }
}

void render_name_menu(app_t *app, menu_t *menu, int w, int h) {
    // -------- Panel --------
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

    SDL_Color titleCol= color_title();
    SDL_Color textCol  = (SDL_Color){240,240,240,255};
    int tw=0, th=0;

    // -------- Título y subtítulo --------
    const int titleY = cardY + 22;
    draw_text_center(app->renderer, app->font, "FLAPPY BIRD", titleCol, cardX + cardW/2, titleY, &tw, &th);

    const int subY = titleY + th + 8;
    draw_text_center(app->renderer, app->font, "Enter your name to star playing", textCol, cardX + cardW/2, subY, NULL, NULL);

    // -------- Bloque de Username centrado verticalmente en la tarjeta --------
    const int padX    = 22;      // padding lateral del input
    const int inputH  = 36;      // alto de la caja de texto
    const int vgap    = 10;      // separación label -> input
    const int helpGap = 12;      // separación input -> texto de ayuda
    const int fh      = TTF_FontHeight(app->font); // alto de una línea de texto

    // Alto total del bloque
    const int blockH  = fh + vgap + inputH + helpGap + fh;

    // Y de inicio para centrar el bloque dentro del panel
    const int startY  = cardY + (cardH - blockH)/2;

    // Caja de entrada
    SDL_Rect input = (SDL_Rect){ cardX + padX, startY + fh + vgap, cardW - 2*padX, inputH };

    // Label
    draw_text_left(app->renderer, app->font, "Username:", textCol,input.x, startY, NULL, NULL);

    // Caja (fondo + borde)
    SDL_SetRenderDrawColor(app->renderer, 20, 30, 40, 110);
    SDL_RenderFillRect(app->renderer, &input);
    SDL_SetRenderDrawColor(app->renderer, 200, 200, 200, 180);
    SDL_RenderDrawRect(app->renderer, &input);

    // Texto 
    const int textPad = 8;
    const char *txt   = (menu->username[0] ? menu->username : "Write your name...");
    SDL_Color txtCol  = (menu->username[0] ? (SDL_Color){255,255,255,255} :(SDL_Color){160,160,160,255});
    draw_text_left(app->renderer, app->font, txt, txtCol, input.x + textPad, input.y + (input.h - 18)/2, &tw, &th);

    // Texto de ayuda, centrado justo bajo el input
    draw_text_center(app->renderer, app->font,"Enter to continue · Esc to exit", textCol, cardX + cardW/2, input.y + inputH + helpGap,NULL, NULL);
}



// ===== tarjeta y lista del menú =====
void draw_menu_list(SDL_Renderer *r, int w, int h, const char **options, int n, int selected, const char *title, const char *subtitle) {
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

void render_skin_menu(app_t *app, menu_t *menu, int w, int h) {
    // Tarjeta base (idéntica a otros menús)
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

    SDL_Color titleCol= color_title();
    SDL_Color textCol  = (SDL_Color){240,240,240,255};
    int tw=0, th=0;

    draw_text_center(app->renderer, app->font, "SELECT SKIN", titleCol, cardX + cardW/2, cardY + 22, &tw, &th);
    draw_text_center(app->renderer, app->font, "Press enter to confirm selection · Esc to return", textCol, cardX + cardW/2, cardY + 22 + th + 8, NULL, NULL);

    // Lista de skins 
    const char* opts[] = { "Angry", "Brainy", "Future", "Purply", "Yellowy"};
    const int n = 5;

    const int lineH = OPTION_LINE_H;
    const int gap   = OPTION_GAP;
    const int optsH = n * lineH + (n - 1) * gap;
    const int startY = cardY + (cardH - optsH)/2 + 16; // centrado vertical simple

    for (int i = 0; i < n; ++i) {
        const int yy = startY + i * (lineH + gap);
        if (i == menu->selected) {
            color_selection(app);
            SDL_Rect hi = { cardX + HILIGHT_INSET, yy - OPTION_HILIGHT_PAD_Y, cardW - 2*HILIGHT_INSET, OPTION_LINE_H + 2*OPTION_HILIGHT_PAD_Y };
            SDL_RenderFillRect(app->renderer, &hi);
        }
        draw_text_center(app->renderer, app->font, opts[i], textCol, cardX + cardW/2, yy, NULL, NULL);
    }

    // --- Preview de la skin seleccionada ---
    // Cacheamos las texturas de preview para no recargar en cada frame
    static SDL_Texture* previews[5] = {NULL,NULL,NULL,NULL,NULL};
    static const char *SKIN_PATHS[5] = {
        "../img/birds/Angry_bird.png",
        "../img/birds/Brain_bird.png",
        "../img/birds/Future_bird.png",
        "../img/birds/Purple_bird.png",
        "../img/birds/Yellow_bird.png"
    };

    int sel = menu->selected;
    if (sel < 0) {
        sel = 0;
    } 
    if (sel > 4) {
        sel = 4;
    }
    if (!previews[sel]) {
        previews[sel] = loadTexture((char*)SKIN_PATHS[sel], app);
    }
    // Zona de preview debajo de la lista
    const int previewMaxW = (int)(cardW * 0.38f);
    const int previewMaxH = (int)(cardH * 0.30f);
    const int endListY    = startY + n * (lineH + gap) - gap;
    const int centerX     = cardX + cardW/2;
    const int topY        = endListY + 20;

    if (previews[sel]) {
        static bird_t p = {0};
        p.scale = 3;
        p.h =(HITBOX_Y)/(p.scale);
        p.w =(HITBOX_X)/(p.scale);
        p.gravity_y = 0.0f;
        p.vel_y = 0;
        int dstX = centerX - p.w / 2;
        int baseY = topY;
        p.x_l = (float)dstX;
        p.x_r = p.x_l + p.w;
        p.y_top = (float)baseY;
        p.y_bottom = p.y_top + p.h;

        // Fondo suave (opcional)
        SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 0);
        SDL_Rect bg = { dstX - 8, baseY - 8, p.w + 16, p.h + 16 };
        SDL_RenderFillRect(app->renderer, &bg);

            // --------- PÁJARO DE PREVIEW ANIMADO (FIJO) ---------
        static int p_inited = 0;
        static int last_sel = -1;
        if (!p_inited || sel != last_sel) {
            memset(&p, 0, sizeof(p));
            p.texture = previews[sel];         // textura de la skin
            p.current_frame = 0;
            p.last_frame_time = SDL_GetTicks();       
            p_inited = 1;
            last_sel = sel;
        } else {
            // Mantener fija la posición y forzar vel_y negativa cada frame
            p.texture = previews[sel];         // por si recargaste assets
        }
        // Avanzar anim de alas y dibujar
        bird_flying(&p);
        draw_bird(&p, app);
    }
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
    SDL_Color titleCol= color_title();
    SDL_Color textCol  = (SDL_Color){240, 240, 240, 255};
    int tw=0, th=0;

    draw_text_center(app->renderer, app->font, "PAUSE - Take a brake ;)", titleCol, cardX + cardW/2, cardY + 22, &tw, &th);
    draw_text_center(app->renderer, app->font, "zzz...", textCol, cardX + cardW/2, cardY + 22 + th + 8, NULL, NULL);

    // opciones
    const char *opts[] = { "Resume", "Restart", "Go back to main menu", "Exit :(" };
    const int n = 4;
    const int lineH = OPTION_LINE_H;
    const int gap   = OPTION_GAP;

    // bloque de opciones al pie 
    const int optsH = n * lineH + (n - 1) * gap;
    const int optYStart = cardY + cardH - PANEL_BOTTOM_PAD - optsH;

    for (int i = 0; i < n; ++i) {
        const int yy = optYStart + i * (lineH + gap);
        if (i == menu->selected) {
            color_selection(app);
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
    SDL_Color titleCol= color_title();
    SDL_Color textCol  = (SDL_Color){240,240,240,255};

    draw_text_center(app->renderer, app->font, "*** GAME OVER ***", titleCol, cardX + cardW/2, cardY + 22, &tw, &th);

    char sub[64];
    snprintf(sub, sizeof(sub), "Score: %d", menu->score);
    draw_text_center(app->renderer, app->font, sub, textCol, cardX + cardW/2, cardY + 22 + th + 8, NULL, NULL);

    int y = cardY + 22 + th + 8;
    // mensaje de felicitación si entró al Top
    if (menu->last_top_pos > 0) {
        char congr[96];
        snprintf(congr, sizeof(congr), "Congratulations, you've made it to TOP %d!", menu->last_top_pos);
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
    const char* opts[] = { "Let's try it again!!", "Exit :(" };
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
            color_selection(app);
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
            color_selection(app);
            SDL_Rect hi = {cardX + HILIGHT_INSET, yy - OPTION_HILIGHT_PAD_Y, cardW - 2*HILIGHT_INSET, OPTION_LINE_H + 2*OPTION_HILIGHT_PAD_Y};
            SDL_RenderFillRect(app->renderer, &hi);
        }
        draw_text_center(app->renderer, app->font, opts[i], (SDL_Color){240,240,240,255}, cardX + cardW/2, yy, NULL, NULL);
    }
}
