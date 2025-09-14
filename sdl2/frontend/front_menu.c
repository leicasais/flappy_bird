#include "front_menu.h"
#include "render_helpers.h"     // draw_text_*
#include "front_main_game.h"    // draw_bird
#include "running_fun.h"        // bird_flying
#include "init_fun.h"           // loadTexture


SDL_Color color_title(void){
    SDL_Color titleCol = (SDL_Color){217, 215, 161, 255};
    return titleCol;
}

void color_selection(app_t *app){
    SDL_SetRenderDrawColor(app->renderer, 107, 119, 142, 180);
}

void render_main_menu(app_t *app, menu_t *menu, int w, int h){
    // 1) Draw the card
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

    // 2) Title and Subtitle
    int tw=0, th=0;
    SDL_Color titleCol= color_title();
    SDL_Color textCol  = (SDL_Color){240, 240, 240, 255};

    draw_text_center(app->renderer, app->font, "FLAPPY", titleCol, cardX + cardW/2, cardY + 22, &tw, &th);

    draw_text_center(app->renderer, app->font, "Press ENTER to start", textCol, cardX + cardW/2, cardY + 22 + th + 10, NULL, NULL);

    // 3) Options
    const char *opts[] = { "Play", "Select Skin" ,"Dificulty" ,"Exit game :(" };
    const int n = NUM_OPTIONS_MAIN;
    const int lineH = ((th) ? (th) + 16 : 40);
    int startY = cardY + cardH/2 - (n * lineH)/2;

    for (int i = 0; i < n; ++i) {
        int y = startY + i * lineH;
        if (i == menu->selected) {
            // Highlight thee selected option
            color_selection(app);
            SDL_Rect hi = { cardX + 20, y - 6, cardW - 40, lineH };
            SDL_RenderFillRect(app->renderer, &hi);
        }
        draw_text_center(app->renderer, app->font, opts[i], textCol, cardX + cardW/2, y, NULL, NULL);
    }
}

void render_dificulty_menu(app_t *app, menu_t *menu, int w, int h){
    // 1) Draw the card
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

    // 2) Title and Subtitle
    int tw=0, th=0;
    SDL_Color titleCol= color_title();
    SDL_Color textCol  = (SDL_Color){240, 240, 240, 255};

    draw_text_center(app->renderer, app->font, "FLAPPY", titleCol, cardX + cardW/2, cardY + 22, &tw, &th);

    draw_text_center(app->renderer, app->font, "Select your Difucult", textCol, cardX + cardW/2, cardY + 22 + th + 10, NULL, NULL);

    // 3) Options
    const char *opts[] = { "Easy","Medium","Hard","Impossible!!" };
    const int n = NUM_OPTIONS_DIFICULTY;
    const int lineH = ((th) ? (th) + 16 : 40);
    int startY = cardY + cardH/2 - (n * lineH)/2;

    for (int i = 0; i < n; ++i) {
        int y = startY + i * lineH;
        if (i == menu->selected) {
            // Highlight the option selected
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

    // -------- Title and subtitle --------
    const int titleY = cardY + 22;
    draw_text_center(app->renderer, app->font, "FLAPPY BIRD", titleCol, cardX + cardW/2, titleY, &tw, &th);

    const int subY = titleY + th + 8;
    draw_text_center(app->renderer, app->font, "Enter your name to star playing", textCol, cardX + cardW/2, subY, NULL, NULL);

    // -------- Username block vertically centered on the card --------
    const int padX    = 22;
    const int inputH  = 36;
    const int vgap    = 10;
    const int helpGap = 12;
    const int fh      = TTF_FontHeight(app->font);
    const int blockH  = fh + vgap + inputH + helpGap + fh;
    const int startY  = cardY + (cardH - blockH)/2;

    // Input box
    SDL_Rect input = (SDL_Rect){ cardX + padX, startY + fh + vgap, cardW - 2*padX, inputH };

    // Label
    draw_text_left(app->renderer, app->font, "Username:", textCol,input.x, startY, NULL, NULL);

    // Box (background + border)
    SDL_SetRenderDrawColor(app->renderer, 20, 30, 40, 110);
    SDL_RenderFillRect(app->renderer, &input);
    SDL_SetRenderDrawColor(app->renderer, 200, 200, 200, 180);
    SDL_RenderDrawRect(app->renderer, &input);

    // Text 
    const int textPad = 8;
    const char *txt   = (menu->username[0] ? menu->username : "Write your name...");
    SDL_Color txtCol  = (menu->username[0] ? (SDL_Color){255,255,255,255} :(SDL_Color){160,160,160,255});
    draw_text_left(app->renderer, app->font, txt, txtCol, input.x + textPad, input.y + (input.h - 18)/2, &tw, &th);

    // Help text, centered just below the input
    draw_text_center(app->renderer, app->font,"Enter to continue · Esc to exit", textCol, cardX + cardW/2, input.y + inputH + helpGap,NULL, NULL);
}


void draw_menu_list(SDL_Renderer *r, int w, int h, const char **options, int n, int selected, const char *title, const char *subtitle) {
    // Centered card
    const int cardW = w * 0.45;
    const int cardH = h * 0.50;
    const int cardX = (w - cardW) / 2;
    const int cardY = (h - cardH) / 2;

    // card background
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 30, 30, 30, 200);
    SDL_Rect card = { cardX, cardY, cardW, cardH };
    SDL_RenderFillRect(r, &card);
    SDL_SetRenderDrawColor(r, 220, 220, 220, 255);
    SDL_RenderDrawRect(r, &card);

    extern app_t app; // if you don't have a global app, pass font/color as parameters
}

void render_skin_menu(app_t *app, menu_t *menu, int w, int h, int shake_x, int shake_y) {
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

    // Title and Subtitle
    draw_text_center(app->renderer, app->font, "SELECT SKIN", titleCol, cardX + cardW/2, cardY + 22, &tw, &th);
    draw_text_center(app->renderer, app->font, "Press enter to confirm selection · Esc to return", textCol, cardX + cardW/2, cardY + 22 + th + 8, NULL, NULL);

    // Skin list  
    const char* opts[] = { "Angry", "Brainy", "Future", "Purply", "Yellowy"};
    const int n = NUM_OPTIONS_SKIN;

    const int lineH = OPTION_LINE_H;
    const int gap   = OPTION_GAP;
    const int optsH = n * lineH + (n - 1) * gap;
    const int startY = cardY + (cardH - optsH)/2 + 16; 

    //Highlight the selected option
    for (int i = 0; i < n; ++i) {
        const int yy = startY + i * (lineH + gap);
        if (i == menu->selected) {
            color_selection(app);
            SDL_Rect hi = { cardX + HILIGHT_INSET, yy - OPTION_HILIGHT_PAD_Y, cardW - 2*HILIGHT_INSET, OPTION_LINE_H + 2*OPTION_HILIGHT_PAD_Y };
            SDL_RenderFillRect(app->renderer, &hi);
        }
        draw_text_center(app->renderer, app->font, opts[i], textCol, cardX + cardW/2, yy, NULL, NULL);
    }

    // --- Preview of the selected skin ---
    // Cache preview textures to avoid reloading each frame
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
    // // Preview area below the list
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

        // Soft background 
        SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 0);
        SDL_Rect bg = { dstX - 8, baseY - 8, p.w + 16, p.h + 16 };
        SDL_RenderFillRect(app->renderer, &bg);

        // --------- ANIMATED PREVIEW BIRD (STATIC POSITION) ---------
        static int p_inited = 0;
        static int last_sel = -1;
        if (!p_inited || sel != last_sel) {
            memset(&p, 0, sizeof(p));
            p.texture = previews[sel];         // Skin Texture
            p.current_frame = 0;
            p.last_frame_time = SDL_GetTicks();       
            p_inited = 1;
            last_sel = sel;
        } 
        else {
            p.texture = previews[sel];         // Keep position fixed and force negative vel_y each frame in case you reloaded assets
        }
        // Advance wing animation and draw
        bird_flying(&p);
        draw_bird(&p, app, shake_x, shake_y);
    }
}

void render_pause_menu(app_t *app, menu_t *menu, int w, int h)
{
    // Card
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

    // Title and Subtitle
    SDL_Color titleCol= color_title();
    SDL_Color textCol  = (SDL_Color){240, 240, 240, 255};
    int tw=0, th=0;

    draw_text_center(app->renderer, app->font, "PAUSE - Take a brake ;)", titleCol, cardX + cardW/2, cardY + 22, &tw, &th);
    draw_text_center(app->renderer, app->font, "zzz...", textCol, cardX + cardW/2, cardY + 22 + th + 8, NULL, NULL);

    // Options
    const char *opts[] = { "Resume", "Restart", "Go back to main menu", "Exit :(" };
    const int n = 4;
    const int lineH = OPTION_LINE_H;
    const int gap   = OPTION_GAP;

    // options block at the bottom 
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

void render_game_over(app_t *app, menu_t *menu, int w, int h){
    // Card
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

    // Title + Score
    int tw=0, th=0;
    SDL_Color titleCol= color_title();
    SDL_Color textCol  = (SDL_Color){240,240,240,255};

    draw_text_center(app->renderer, app->font, "*** GAME OVER ***", titleCol, cardX + cardW/2, cardY + 22, &tw, &th);

    char sub[64];
    snprintf(sub, sizeof(sub), "Score: %d", menu->score);
    draw_text_center(app->renderer, app->font, sub, textCol, cardX + cardW/2, cardY + 22 + th + 8, NULL, NULL);
    int y = cardY + 22 + th + 8;

    // Congratulatory message if it made the Top
    if (menu->last_top_pos > 0){
        char congr[96];
        snprintf(congr, sizeof(congr), "Congratulations, you've made it to TOP %d!", menu->last_top_pos);
        y += 22;
        draw_text_center(app->renderer, app->font, congr, textCol, cardX + cardW/2, y, NULL, NULL);
    }

    // Table title
    y += 28;
    draw_text_center(app->renderer, app->font, "TOP 10", titleCol, cardX + cardW/2, y, NULL, NULL);


                                    // --- TOP-10 LIST ---
    y += 20;                            
    const int listX = cardX + 40;      
    const char* opts[] = { "Let's try it again!!", "Exit :(" };
    const int n = NUM_OPTIONS_GAME_OVER;
    const int lineH = OPTION_LINE_H;
    const int gap   = OPTION_GAP;
    const int optsH = n * lineH + (n - 1) * gap;
    const int optYStart = cardY + cardH - PANEL_BOTTOM_PAD - optsH;

    // Available height for 10 rows
    int listStartY = y;
    int availH = optYStart - listStartY - LIST_OPTIONS_GAP;    
    if (availH < 10){
        availH = 10;
    }

    // Base font height and scale so that the 10 rows fit
    int baseH  = TTF_FontHeight(app->font);
    int rowH   = availH / MAX_SCORES;               
    float scale = (float)rowH / (float)baseH;       
    if (scale > 0.95f){
        scale = 0.95f;               
    }
    if (scale < 0.65f){
        scale = 0.65f;          
    }

    // Adjust rowH according to actual scale and leave 2 px of inner padding
    int text_h = (int)(baseH * scale);
    rowH = text_h + 4;

    for (int i = 0; i < MAX_SCORES; ++i){
        const int rowY = listStartY + i * rowH;

        // Highlight the entire row if the new score landed in that position
        if (menu->last_top_pos == i + 1){
            color_selection(app);
            SDL_Rect hi = { cardX + 20, rowY - 2, cardW - 40, rowH };
            SDL_RenderFillRect(app->renderer, &hi);
        }

        char line[48];
        snprintf(line, sizeof(line), "%2d) %d", i + 1, menu->high_score[i]);

        /// Draw the text vertically centered within the row
        int tw=0, th=0;
        draw_text_left_scaled(app->renderer, app->font, line, textCol, listX, rowY + (rowH - text_h)/2, scale, &tw, &th);
    }

    // --- Options at the bottom ---
    for (int i = 0; i < n; ++i){
        const int yy = optYStart + i * (lineH + gap);
        if (i == menu->selected){
            color_selection(app);
            SDL_Rect hi = {cardX + HILIGHT_INSET, yy - OPTION_HILIGHT_PAD_Y, cardW - 2*HILIGHT_INSET, OPTION_LINE_H + 2*OPTION_HILIGHT_PAD_Y};
            SDL_RenderFillRect(app->renderer, &hi);
        }
        draw_text_center(app->renderer, app->font, opts[i], (SDL_Color){240,240,240,255}, cardX + cardW/2, yy, NULL, NULL);
    }
}
