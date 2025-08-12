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
void prepareScene(app_t *app)   //Prepares a clean background for drawing the next frame.
{
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
    SDL_Rect dest_rect = {.x=bird->x_l, .y=bird->y_top, .w= (bird->x_r - bird->x_l), .h= bird->y_bottom - bird->y_top} ; 
    SDL_RenderCopy(app->renderer, bird->texture, &scr_rect, &dest_rect);       // Copies a part of a texture (src_rect) and draws it in the screen using the active render



}

// sin estirar en vertical: repite la textura en bloques y recorta el último si hace falta.
static void draw_tiled_segment(SDL_Renderer *r, SDL_Texture *tex, int x, int src_x, int y, int w, int src_w, int h)
{
    if (h <= 0 || w <= 0 || !tex) return;

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
            if(pcol[i].x != OUTSIDE){
                px_per_len= (COL_PX_W/COL_WIDTH)* (pcol[i].len);
                SDL_Rect src_rect = {.y =0, .h =COL_PX_H, .w=px_per_len};
                if(pcol[i].x==0 ){
                    src_x= COL_PX_W-(px_per_len);
                }
                else{
                    src_x=0;
                }

                //Displays the top of the column of the sup part
                int up_x=pcol[i].x;
                int up_y=0;
                int up_w=COL_WIDTH-(COL_WIDTH-pcol[i].len);
                int up_h=pcol[i].y;
                draw_tiled_segment(app->renderer, pcol[i].texture_up, up_x,src_x, up_y, up_w,px_per_len, up_h);
 
                //Displays the bottom of the column of the inferior part
                int down_x=pcol[i].x;
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

// ====== UI y Menús ======
#if __has_include(<SDL2/SDL_ttf.h>)
  #include <SDL2/SDL_ttf.h>
  #define HAVE_TTF 1
#elif __has_include(<SDL_ttf.h>)
  #include <SDL_ttf.h>
  #define HAVE_TTF 1
#else
  #define HAVE_TTF 0
#endif

static int g_ttf_ok = 0;
#if HAVE_TTF
static TTF_Font *g_font = NULL;
#endif

static void set_blend(SDL_Renderer *r){ SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND); }
static void fill_rect(SDL_Renderer *r, SDL_Rect rc, Uint8 R, Uint8 G, Uint8 B, Uint8 A){
    set_blend(r); SDL_SetRenderDrawColor(r, R,G,B,A); SDL_RenderFillRect(r, &rc);
}
static void draw_rect(SDL_Renderer *r, SDL_Rect rc, Uint8 R, Uint8 G, Uint8 B, Uint8 A){
    set_blend(r); SDL_SetRenderDrawColor(r, R,G,B,A); SDL_RenderDrawRect(r, &rc);
}

#if HAVE_TTF
static void draw_text(SDL_Renderer *ren, const char *msg, int x, int y, SDL_Color c){
    if (!g_ttf_ok || !g_font || !msg) return;
    SDL_Surface *s = TTF_RenderUTF8_Blended(g_font, msg, c);
    if (!s) return;
    SDL_Texture *t = SDL_CreateTextureFromSurface(ren, s);
    int w = s->w, h = s->h;
    SDL_FreeSurface(s);
    if (!t) return;
    SDL_Rect dst = {x,y,w,h};
    SDL_RenderCopy(ren, t, NULL, &dst);
    SDL_DestroyTexture(t);
}
#endif

int ui_init(app_t *app, const char *ttf_path, int pt_size){
    (void)app;
#if HAVE_TTF
    if (TTF_WasInit() == 0){
        if (TTF_Init() == -1){ g_ttf_ok = 0; return -1; }
    }
    g_font = TTF_OpenFont(ttf_path ? ttf_path : "assets/DejaVuSans.ttf", pt_size > 0 ? pt_size : 28);
    if (!g_font){ g_ttf_ok = 0; return -1; }
    g_ttf_ok = 1;
    return 0;
#else
    g_ttf_ok = 0;
    return -1;
#endif
}

void ui_shutdown(void){
#if HAVE_TTF
    if (g_font){ TTF_CloseFont(g_font); g_font = NULL; }
    if (TTF_WasInit()) TTF_Quit();
#endif
    g_ttf_ok = 0;
}

static SDL_Rect draw_menu_list(SDL_Renderer *ren, int w, int h,
                               const char **options, int n, int selected,
                               const char *title, const char *subtitle)
{
    const int line_h = 44, pad_x = 32, pad_y = 28, gap = 14;
    int card_w = (w > 480) ? w/2 : (w - 40);
    int content_h = pad_y + (title? line_h:0) + (subtitle? (line_h-10):0) + gap
                  + n*line_h + (n-1)*gap + pad_y;
    int card_h = (content_h > h - 60) ? (h - 60) : content_h;
    SDL_Rect card = { (w - card_w)/2, (h - card_h)/2, card_w, card_h };

    // overlay y tarjeta
    fill_rect(ren, (SDL_Rect){0,0,w,h}, 0,0,0,160);
    fill_rect(ren, card,  20,24,28, 235);
    draw_rect(ren, card,  255,255,255, 40);

    int x = card.x + pad_x, y = card.y + pad_y;
#if HAVE_TTF
    SDL_Color title_col = {245,245,245,255};
    SDL_Color text_col  = {220,220,220,255};
    SDL_Color hint_col  = {180,180,180,255};
    if (title){ draw_text(ren, title, x, y, title_col); y += line_h; }
    if (subtitle){ draw_text(ren, subtitle, x, y-6, hint_col); y += (line_h - 10) + gap; } else y += gap;
#endif

    for (int i = 0; i < n; ++i){
        SDL_Rect row = { x-8, y-6, card.w - 2*pad_x + 16, line_h };
        if (i == selected) { fill_rect(ren, row, 70,120,255,60); draw_rect(ren, row, 100,160,255,180); }
        else               { draw_rect(ren, row, 255,255,255,30); }
#if HAVE_TTF
        if (options && options[i]) draw_text(ren, options[i], x, y-2, text_col);
#endif
        y += line_h + gap;
    }
#if HAVE_TTF
    draw_text(ren, "↑/↓ mover  •  Enter seleccionar  •  Esc volver",
              card.x + pad_x, card.y + card.h - pad_y, (SDL_Color){180,180,180,255});
#endif
    return card;
}

void render_main_menu(app_t *app, menu_t *menu, int w, int h){
    const char *opts[] = { "Jugar", "Salir" };
    draw_menu_list(app->renderer, w, h, opts, 2, menu->selected, "FLAPPY", "Presiona Enter para comenzar");
}

void render_pause_menu(app_t *app, menu_t *menu, int w, int h){
    const char *opts[] = { "Continuar", "Reiniciar", "Menu Principal", "Salir" };
    draw_menu_list(app->renderer, w, h, opts, 4, menu->selected, "PAUSA", "Esc para volver");
}

void render_game_over(app_t *app, menu_t *menu, int w, int h){
    char sub[128]; int best = menu->high_score[0];
    snprintf(sub, sizeof(sub), "Puntaje: %d   |   Record: %d", menu->score, best);
    const char *opts[] = { "Reintentar", "Menu Principal", "Salir" };
    draw_menu_list(app->renderer, w, h, opts, 3, menu->selected, "GAME OVER", sub);
}

void render_game_hud(app_t *app, menu_t *menu){
#if HAVE_TTF
    char txt[64]; snprintf(txt, sizeof(txt), "Score: %d", menu->score);
    draw_text(app->renderer, txt, 16, 12, (SDL_Color){255,255,255,255});
#endif
}
