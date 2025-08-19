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

//windows
void prepareScene(app_t *app){   //Prepares a clean background for drawing the next frame.
    SDL_SetRenderDrawColor(app->renderer,106, 138, 153, 255);   //Sets the color that SDL will use when clearing or drawing, SDL_SetRenderDrawColor(app.renderer, R, G, B, A); A: opacidad (255 ultra opaco)
    SDL_RenderClear(app->renderer);         //Clears the screen with the color just set. erease the previous frame
}

void draw_bird(bird_t *bird, app_t *app){
    int fly_frames[] = {0, 6, 6, 7};
    //&srcRect the position and size in pixeles that you want to cut off the img
    SDL_Rect scr_rect;
    int frame_index = fly_frames[bird->current_frame];
    scr_rect.x = (frame_index % 4) * HITBOX_X;          //gives the index of the column of the bird matrix
    scr_rect.y = (frame_index / 4) * HITBOX_Y;
    scr_rect.w = HITBOX_X;
    scr_rect.h = HITBOX_Y;

    // &destRect sets the size of the cut img and where to plaice it in the screen
    SDL_Rect dest_rect = {.x = (int)bird->x_l, .y = (int)bird->y_top, .w = (int)(bird->x_r - bird->x_l), .h = (int)(bird->y_bottom - bird->y_top)};
    SDL_RenderCopyEx(app->renderer, bird->texture, &scr_rect, &dest_rect, bird->vel_y*5,NULL, SDL_FLIP_NONE);       // Copies a part of a texture (src_rect) and draws it in the screen using the active render



}

// sin estirar en vertical: repite la textura en bloques y recorta el último si hace falta.
static void draw_tiled_segment(SDL_Renderer *r, SDL_Texture *tex, int x, int src_x, int y, int w, int src_w, int h){
    if (h <= 0 || w <= 0 || !tex) {
        return;
    }

    int texW, texH;
    SDL_QueryTexture(tex, NULL, NULL, &texW, &texH);

    int drawn = 0;
    while (drawn < h) {
        int chunk = (h - drawn < texH) ? (h - drawn) : texH;

        SDL_Rect src = {src_x, 0, src_w, chunk};        // recorto solo lo que entra
        SDL_Rect dst = {x, y + drawn, w, chunk};   // mismo alto que src -> sin estirar verticalmente
        SDL_RenderCopy(r, tex, &src, &dst);

        drawn += chunk;
    }
}

void draw_col(column_t* pcol, app_t *app){
    for(int i=0; i< NUM_COL; i++){
        int src_x;
        int px_per_len;
        if(pcol[i].x != OUTSIDE) {
            px_per_len= (COL_PX_W/COL_WIDTH)* (pcol[i].len);
            SDL_Rect src_rect = {.y =0, .h =COL_PX_H, .w=px_per_len};
            if((int)pcol[i].x==0 ) {
                src_x= COL_PX_W-(px_per_len);
            }
            else{
                src_x=0;
            }
                
            //Displays the top of the column of the sup part
            int up_x=(int)pcol[i].x;
            int up_y=0;
            int up_w=COL_WIDTH-(COL_WIDTH-pcol[i].len);
            int up_h=pcol[i].y;
            draw_tiled_segment(app->renderer, pcol[i].texture_up, up_x,src_x, up_y, up_w,px_per_len, up_h); 
                
            //Displays the bottom of the column of the inferior part
            int down_x=(int)pcol[i].x;
            int down_y=pcol[i].y + HOLE_HEIGHT;
            int down_w=COL_WIDTH-(COL_WIDTH-pcol[i].len);
            int down_h = (GAME_HEIGHT-TILE_HIGHT-1) - (pcol[i].y + HOLE_HEIGHT);
            draw_tiled_segment(app->renderer, pcol[i].texture_down, down_x,src_x, down_y, down_w,px_per_len , down_h);
        }
    }
}


void draw_background(background_t *background, app_t *app){
    SDL_Rect dest_tile = {.x = 0, .y= GAME_HEIGHT-TILE_HIGHT-1, .w= GAME_WIDTH, .h = TILE_HIGHT};
    SDL_RenderCopy(app->renderer, background->tile_tex, NULL, &dest_tile);
}

// frontend.c
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

void render_game_hud(app_t *app, menu_t *menu){
    char buf[32]; 
    snprintf(buf, sizeof(buf), "Score: %d", menu->score);
    SDL_Color c = { 250, 250, 250, 255 };
    draw_text_center(app->renderer, app->font, buf, c, 60, 8, NULL, NULL);
}

static SDL_Texture* make_text(SDL_Renderer* r, TTF_Font* font, const char* txt, SDL_Color color, int* w, int* h) {
    SDL_Surface* s = TTF_RenderUTF8_Blended(font, txt, color);
    if (!s) { 
        SDL_Log("TTF_Render FAIL: %s", TTF_GetError()); 
        return NULL; 
    }
    SDL_Texture* t = SDL_CreateTextureFromSurface(r, s);
    if (w) { 
        *w = s->w; if (h) *h = s->h;
    }
    SDL_FreeSurface(s);
    SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
    return t;
}

void draw_text_center(SDL_Renderer* r, TTF_Font* font, const char* text, SDL_Color color, int cx, int y, int* out_w, int* out_h) {
    int w=0, h=0;
    SDL_Texture* tex = make_text(r, font, text, color, &w, &h);
    if (!tex) {
        return;
    }
    SDL_Rect dst = { cx - w/2, y, w, h };
    SDL_RenderCopy(r, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
    if (out_w) {
        *out_w = w; if (out_h) *out_h = h;
    }
}

static void draw_text_left(SDL_Renderer* r, TTF_Font* font, const char* text, SDL_Color color, int x, int y, int* out_w, int* out_h) {
    int w=0, h=0;
    SDL_Texture* tex = make_text(r, font, text, color, &w, &h);
    if (!tex) {
        return;
    }
    SDL_Rect dst = { x, y, w, h };
    SDL_RenderCopy(r, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
    if (out_w) {
        *out_w = w; if (out_h) *out_h = h;
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

    // lista Top 10 alineada a la izquierda
    y += 20;
    int listX = cardX + 40;
    for (int i = 0; i < MAX_SCORES; ++i) {
        char line[48];
        snprintf(line, sizeof(line), "%2d) %d", i+1, menu->high_score[i]);

        if (menu->last_top_pos == i+1) {
            SDL_SetRenderDrawColor(app->renderer, 70, 120, 200, 120);
            SDL_Rect hi = { cardX + 20, y - 4, cardW - 40, 20 };
            SDL_RenderFillRect(app->renderer, &hi);
        }
        draw_text_left(app->renderer, app->font, line, textCol, listX, y, NULL, NULL);
        y += 20;
    }

    // opciones al pie
    const char* opts[] = { "Volver a jugar", "Salir" };
    const int n = 2;
    const int lineH = OPTION_LINE_H;
    const int gap   = OPTION_GAP;

    // alto total del bloque de opciones (2 botones + separación)
    const int optsH = n * lineH + (n - 1) * gap;

    // arranque del bloque de opciones dejando un margen inferior claro
    const int optYStart = cardY + cardH - PANEL_BOTTOM_PAD - optsH;

    for (int i = 0; i < n; ++i) {
        const int yy = optYStart + i * (lineH + gap);

        if (i == menu->selected) {
            SDL_SetRenderDrawColor(app->renderer, 70, 120, 200, 180);
            SDL_Rect hi = { cardX + HILIGHT_INSET, yy - 6, cardW - 2*HILIGHT_INSET, lineH };
            SDL_RenderFillRect(app->renderer, &hi);
        }
        draw_text_center(app->renderer, app->font, opts[i], (SDL_Color){240,240,240,255}, cardX + cardW/2, yy, NULL, NULL);
    }
}






/*void display_upper_line(menu_t menu) {
    mvprintw(0, 0, "Lives: ");
    for (int i = 0; i < menu.lives; i++) {
        printw(" <3 "); // harts
    }
    char str[20];
    sprintf(str, "Score: %d", menu.score);  // Convierte el número en texto
    mvprintw(0, GAME_WIDTH-strlen(str)-1, "%s", str);
}

void displayy_bird(bird_t * bird, int ch){
    static int up=0;
    if(ch==' '){
        up=!up;
    }
    else if(bird->delta_y>0){
        mvaddch(bird->y,bird->x,'~');
    }
    else if(up){
            mvaddch(bird->y,bird->x,'v');
        }
        else{
            mvaddch(bird->y,bird->x,'^');
        }
    
}

void display_resurecting_bird(bird_t * bird, int ch){
    static int up=0;
    static int time=0;
    if(ch==' '){
        up=!up;
    }
    if(time<10){
        if(up){
            mvaddch(bird->y,bird->x,'o');
        }
        else{
            mvaddch(bird->y,bird->x,'-');
        }
    }
    if(time<20){
        time++;
    }
    else{
        time=0;
    }  
    
}*/


