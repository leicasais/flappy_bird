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
    init(column, &bird, &menu, &app, &background);            // update the initial conditions of the game
    init_tex(column, &bird, &menu, &app, &background);        //charges the textures 

    // --- Menú: estado inicial + fuente para UI del menú ---
   // menu_init(&menu);                                         // MAIN_MENU + highscores :contentReference[oaicite:6]{index=6}

    int running = 1;
    int reboot_time=0; 
    while (running){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){ 
                running = 0; 
                break; 
            }
            if (event.type == SDL_KEYDOWN && event.key.repeat == 0){
                switch (menu.state){
                    case BEGINING: 
                        if(event.key.keysym.sym == SDLK_SPACE){ //press space to begin playing
                            menu.state=RUNING;
                        }
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
            if(reboot_time==0){
                points(column, &bird, &menu);       //Adds a point if the bird has passed a column only if in game mode
            }
                
            if (collision(column, &bird) && !reboot_time){
                colition_update(&menu);//counts the collition only if the game isnt in reboot mode
                if(menu.state != GAME_OVER){
                    reboot_time=1;
                }
            }
            else if(reboot_time > 0 && reboot_time < (MS_BTW_FRAMES * (column->col_speed)*2)){
                reboot_time++;
                // show resurrection animation
            }
            else{
                reboot_time = 0;                //Ends the resurection mode
            }
        }

        // --- Render ---
        prepareScene(&app);
        draw_background(&background, &app);

        if (menu.state == MAIN_MENU) {
            render_main_menu(&app, &menu, GAME_WIDTH, GAME_HEIGHT); // "FLAPPY" + Play/Salir
        } 
        else if (menu.state == RUNING || menu.state == BEGINING) {
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

    cleanupSDL(&app, &bird, column, &background, &menu);
    free(column);
    return 0;
}
