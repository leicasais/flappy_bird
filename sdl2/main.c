#include "structs.h"
#include "front_main_game.h"
#include "front_menu.h"
#include "game_logic.h"
#include "init_fun.h"
#include "menu.h"
#include "render_helpers.h"
#include "running_fun.h"


int main(void){
    bird_t bird;
    menu_t menu;
    app_t app;
    background_t background;
    screen_dim_t screen_dim;
    column_t* column;

    init_parameters(&screen_dim);
    column = malloc(sizeof(column_t) * screen_dim.NUM_COL);

    memset(&app, 0, sizeof(app));
    SDL_StartTextInput();
    initSDL(&app, &screen_dim);

    menu_init(&menu); 

    // Objetos y texturas (columnas, pájaro, background, fuente para HUD)
    init(column, &bird, &menu, &screen_dim);            // update the initial conditions of the game
    init_tex(column, &bird, &menu, &app, &background, &screen_dim);        //charges the textures 


    int running = 1;
    int reboot_time=0; 
    while (running){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){ 
                running = 0; 
                break; 
            }
            if (event.type == SDL_TEXTINPUT) {
                if (menu.state == NAME_MENU && menu.name_editing) {
                    for (const char *p = event.text.text; *p; ++p) {
                        unsigned char c = (unsigned char)*p;
                        if (c >= ' ' && c <= '~') {                // imprimibles
                            size_t len = strlen(menu.username);
                            if (len < USERNAME_MAX) {
                                menu.username[len++]   = (char)c;
                                menu.username[len] = '\0';
                            }
                        }
                    }
                }
                continue;
            }

            if (event.type == SDL_KEYDOWN && event.key.repeat == 0){
                switch (menu.state){
                    case BEGINING: 
                        if(event.key.keysym.sym == SDLK_SPACE){ //press space to begin playing
                            menu.state=RUNING;
                        }
                        else if (event.key.keysym.sym == SDLK_ESCAPE){
                            menu_set_state(&menu, PAUSE);
                        }
                    break;

                    case NAME_MENU:
                        if (event.key.keysym.sym == SDLK_BACKSPACE) {
                            size_t len = strlen(menu.username);
                            if (len > 0) {
                                menu.username[len-1] = '\0';
                            }
                        } 
                        else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                            if (menu.username[0]) {              
                                menu.name_editing = 0;
                                menu_set_state(&menu, MAIN_MENU);
                            }
                        } 
                        else if (event.key.keysym.sym == SDLK_ESCAPE) {
                            menu_set_state(&menu, EXIT);
                            running = 0;
                        }
                    break;

                    case MAIN_MENU:
                        if (event.key.keysym.sym == SDLK_UP){      
                            menu_prev_option(&menu);
                        }
                        else if (event.key.keysym.sym == SDLK_DOWN){  
                            menu_next_option(&menu);
                        }
                        else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
                            // Play o Salir
                            menu_activate_selected(&menu, column, &bird, &app, &screen_dim);
                            if (menu.state == EXIT){
                                running = 0;
                            } 
                            else if (event.key.keysym.sym == SDLK_ESCAPE){
                                running = 0;
                            }
                        }
                    break;

                    case SKIN_MENU:
                        if (event.key.keysym.sym == SDLK_UP) {
                            menu_prev_option(&menu);
                        }
                        else if (event.key.keysym.sym == SDLK_DOWN) {
                            menu_next_option(&menu);
                        }
                        else if (event.key.keysym.sym == SDLK_RETURN ||event.key.keysym.sym == SDLK_KP_ENTER) {
                            menu_activate_selected(&menu, column, &bird, &app, &screen_dim);   // aplica skin y vuelve al Main
                        }
                        else if (event.key.keysym.sym == SDLK_ESCAPE) {
                            menu_set_state(&menu, MAIN_MENU);                      // cancelar y volver
                        }
                    break;

                    case DIFICULTY_MENU:
                        if (event.key.keysym.sym == SDLK_UP) {
                            menu_prev_option(&menu);
                        }
                        else if (event.key.keysym.sym == SDLK_DOWN) {
                            menu_next_option(&menu);
                        }
                        else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                            menu_activate_selected(&menu, column, &bird, &app, &screen_dim);
                        }
                        else if (event.key.keysym.sym == SDLK_ESCAPE) {
                            menu_set_state(&menu, MAIN_MENU);                      // cancelar y volver
                        }
                    break;

                    case RUNING:
                        if (event.key.keysym.sym == SDLK_SPACE){
                            bird.vel_y = -8.0f; 
                        } 
                        else if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == 'p' || event.key.keysym.sym == 'P'){
                            menu_set_state(&menu, PAUSE);
                        }
                    break;

                    case PAUSE:
                        if (event.key.keysym.sym == SDLK_UP) {
                            menu_prev_option(&menu);
                        }
                        else if (event.key.keysym.sym == SDLK_DOWN) {
                            menu_next_option(&menu);
                        }
                        else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                            menu_activate_selected(&menu, column, &bird, &app, &screen_dim);
                            if (menu.state == EXIT) {
                                running = 0;
                            }
                            if(menu.state == BEGINING){     //if restart is selected
                                reboot_time=0;              //it doesnt begin with resurecting mode
                                bird.vel_y = 0;             //sets the bird velocity on cero again
                            }
                        }
                        else if (event.key.keysym.sym == SDLK_ESCAPE) {
                            menu_set_state(&menu, RUNING);   // Esc reanuda sin elegir
                        }
                    break;


                    case GAME_OVER:
                        reboot_time=0;
                        bird.vel_y=0;
                        if (event.key.keysym.sym == SDLK_UP) {         
                            menu_prev_option(&menu);
                        }
                        else if (event.key.keysym.sym == SDLK_DOWN) { 
                            menu_next_option(&menu);
                        }
                        else if (event.key.keysym.sym == SDLK_RETURN ||event.key.keysym.sym == SDLK_KP_ENTER) {
                            menu_activate_selected(&menu, column, &bird, &app, &screen_dim);
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
            col_mov(column, &screen_dim, &menu);
            bird_fall(&bird, &screen_dim);
            update_bird_animation(&bird);
            if(reboot_time==0){
                points(column, &bird, &menu, &screen_dim);       //Adds a point if the bird has passed a column only if in game mode
            }
                
            if (collision(column, &bird, &screen_dim) && !reboot_time){
                colition_update(&menu);//counts the collition only if the game isnt in reboot mode
                if(menu.state != GAME_OVER){
                    reboot_time=1;
                }
            }
            else if(reboot_time > 0 && reboot_time < (MS_BTW_FRAMES * (column->col_speed)*4)){
                reboot_time++;
                // show resurrection animation
            }
            else{
                reboot_time = 0;                //Ends the resurection mode
            }
        }

        // --- Render ---
        prepareScene(&app);
        draw_background(&background, &app, &screen_dim);

        if (menu.state == NAME_MENU) {
            render_name_menu(&app, &menu, screen_dim.GAME_WIDTH, screen_dim.GAME_HEIGHT);
        }
        else if (menu.state == MAIN_MENU) {
            render_main_menu(&app, &menu, screen_dim.GAME_WIDTH, screen_dim.GAME_HEIGHT); // "FLAPPY" + Play/Salir
            reboot_time=0;      //acordate de agregar esta linea en la opcion restart del menu de pausa
        } 
        else if (menu.state == SKIN_MENU) {
            render_skin_menu(&app, &menu, screen_dim.GAME_WIDTH, screen_dim.GAME_HEIGHT);
        }
        else if(menu.state == DIFICULTY_MENU){
            render_dificulty_menu(&app, &menu, screen_dim.GAME_WIDTH, screen_dim.GAME_HEIGHT);
        }
        else if (menu.state == RUNING || menu.state == BEGINING) {
            draw_col(column, &app, &screen_dim);
            if(!reboot_time){
                draw_bird(&bird, &app);
                draw_hearts(&app, &menu);
            }
            else{
                display_resurecting(&bird, &menu, &app);
            }
            render_game_hud(&app, &menu, &screen_dim);
            
        }
        else if (menu.state == PAUSE) {
            draw_col(column, &app, &screen_dim);
            if(!reboot_time){
                draw_bird(&bird, &app);
                draw_hearts(&app, &menu);
            } 
            else {
                display_resurecting(&bird, &menu, &app);
            }
            render_game_hud(&app, &menu, &screen_dim);
            render_pause_menu(&app, &menu, screen_dim.GAME_WIDTH, screen_dim.GAME_HEIGHT);
        }
        else if (menu.state == GAME_OVER) {
            draw_col(column, &app, &screen_dim);
            draw_bird(&bird, &app);
            render_game_over(&app, &menu, screen_dim.GAME_WIDTH, screen_dim.GAME_HEIGHT);
        }

        SDL_RenderPresent(app.renderer);
        SDL_Delay(16);
    }

    cleanupSDL(&app, &bird, column, &background, &menu, &screen_dim);
    free(column);
    return 0;
}
