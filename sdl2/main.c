#include "backend.h"
#include "frontend.h"

//Variables Globales
int GAME_WIDTH;
int GAME_HEIGHT;
int TILE_HIGHT;
int HOLE_HEIGHT;
int NUM_COL;
int COL_WIDTH;
int SPACE;                  // px por frame (dinámica)

column_t* column;

int main(void){
    bird_t bird;
    menu_t menu;
    app_t app;
    background_t background;

    init_parameters();
    column = malloc(sizeof(column_t) * NUM_COL);

    memset(&app, 0, sizeof(app));
    initSDL(&app);

    // Objetos y texturas (columnas, pájaro, background, fuente para HUD)
    init(column, &bird, &menu, &app, &background);            // carga texturas, etc. :contentReference[oaicite:5]{index=5}

    // --- Menú: estado inicial + fuente para UI del menú ---
    menu_init(&menu);                                         // MAIN_MENU + highscores :contentReference[oaicite:6]{index=6}

    int running = 1;
    while (running){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){ 
                running = 0; 
                break; 
            }
            if (event.type == SDL_KEYDOWN && event.key.repeat == 0){
                switch (menu.state){
                    case MAIN_MENU:
                        if (event.key.keysym.sym == SDLK_UP){      
                            menu_prev_option(&menu);
                        }
                        else if (event.key.keysym.sym == SDLK_DOWN){  
                            menu_next_option(&menu);
                        }
                        else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
                            // Play o Salir
                            menu_activate_selected(&menu, column, &bird, &app);
                            if (menu.state == EXIT){
                                running = 0;
                            } 
                            else if (event.key.keysym.sym == SDLK_ESCAPE){
                                running = 0;
                            }
                        }
                        break;

                    case RUNING:
                        if (event.key.keysym.sym == SDLK_SPACE){
                            bird.vel_y = -8.0f; // salto
                        } 
                        else if (event.key.keysym.sym == SDLK_ESCAPE){
                            // si querés: volver al menú con ESC
                            menu_set_state(&menu, MAIN_MENU);
                        }
                        break;

                    case GAME_OVER:
                        if (event.key.keysym.sym == SDLK_UP) {         
                            menu_prev_option(&menu);
                        }
                        else if (event.key.keysym.sym == SDLK_DOWN) { 
                            menu_next_option(&menu);
                        }
                        else if (event.key.keysym.sym == SDLK_RETURN ||event.key.keysym.sym == SDLK_KP_ENTER) {
                            menu_activate_selected(&menu, column, &bird, &app);
                            if (menu.state == EXIT) {
                                running = 0;
                            }
                        } 
                        else if (event.key.keysym.sym == SDLK_ESCAPE) {
                            menu_set_state(&menu, EXIT);
                            running = 0;
                        }
                        break;

                    case EXIT:
                        running = 0;
                        break;

                    default:
                        break;
                }
            }
        }

        // --- Update por estado ---
        if (menu.state == RUNING){
            col_mov(column);
            bird_fall(&bird);
            update_bird_animation(&bird);
            points(column, &bird, &menu);
                
            if (collision(column, &bird)){
                // acá podrías ir a GAME_OVER si querés; por ahora volvemos al menú
                menu.last_top_pos = score_update(&menu,menu.score);
                score_save(&menu);
                menu_set_state(&menu, GAME_OVER);
            }
        }

        // --- Render ---
        prepareScene(&app);
        draw_background(&background, &app);

        if (menu.state == MAIN_MENU) {
            render_main_menu(&app, &menu, GAME_WIDTH, GAME_HEIGHT); // "FLAPPY" + Play/Salir
        } 
        else if (menu.state == RUNING) {
            draw_col(column, &app);
            draw_bird(&bird, &app);
            render_game_hud(&app, &menu);
        }
        else if (menu.state == GAME_OVER) {
            // Podés dibujar el mundo “congelado” debajo si querés
            draw_col(column, &app);
            draw_bird(&bird, &app);
            render_game_over(&app, &menu, GAME_WIDTH, GAME_HEIGHT);
        }

        SDL_RenderPresent(app.renderer);
        SDL_Delay(16);
    }

    cleanupSDL(&app, &bird, column, &background);
    free(column);
    return 0;
}
