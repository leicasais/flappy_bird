//std libs and SDL2
#ifndef EXTERN_LIBS_H
#define EXTERN_LIBS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
    #include <time.h>
    #include <unistd.h>  // usleep()
#endif

#ifndef BACKEND_H
#define BACKEND_H

    /*#########################################
                        MACROS
    #########################################*/

    //Macros for the main/game flow
    #define MS_BTW_FRAMES 16
    #define VEL_COL 4
    #define MIN_GAME_HEIGHT 8
    #define MIN_GAME_WIDTH 8 

    //macros for the columns
    #define OUTSIDE -1000
    #define COL_PX_H 1024
    #define COL_PX_W 256

    //macros birds
    #define HITBOX_X 15
    #define HITBOX_Y 16
    
    //macros for the menus
    #define NUM_OPTIONS_MAIN 2 
    #define NUM_OPTIONS_GAME_OVER 3
    #define NUM_OPTIONS_PAUSE 4
    #define MAIN_MENU 1
    #define RUNING 2
    #define PAUSE 3
    #define CONTINUE 4
    #define EXIT 5
    #define RESTART 6
    #define GAME_OVER 7
    #define BEGINING 8
    #define MAX_SCORES 10

    /*#########################################
                        Structs
    #########################################*/

    typedef struct{
        SDL_Renderer *renderer;
        SDL_Window *window;
        TTF_Font    *font;
        SDL_Texture *score_tex;
        int          score_w, score_h;
        SDL_Color    score_color;
    }app_t;
    
    typedef struct{
        SDL_Texture *tile_tex;
    }background_t;

    typedef struct{ //Origin in (1,1)    
        int x;
        int y;
        int len;
        SDL_Texture *texture_down;
        SDL_Texture *texture_up;
        int trim;
        int current_frame;
        Uint32 last_frame_time;
    }column_t;

    typedef struct{
        float y_top;
        float y_bottom; //CAMBIE TODO A FLOAT, GUARDA!!! REVISALO BIEN
        float x_l;
        float x_r;
        float gravity_y;
        float vel_y;
        int scale;
        SDL_Texture *texture;
        int current_frame;
        Uint32 last_frame_time;
    }bird_t;

    typedef struct {
        int score;
        int lives;
        int high_score[MAX_SCORES];
        int state;          // MAIN_MENU, RUNING, PAUSE, GAME_OVER, etc.
        int selected;       // índice de opción actualmente seleccionada en el menú activo
    } menu_t;

    /*#########################################
                        Functions
    #########################################*/
    
    //init_fun.c
    void init_parameters(void);
    void init(column_t* pcol, bird_t *bird, menu_t *menu, app_t *app, background_t *background);    void initSDL(app_t *app);
    void set_parameters(void);
    void cleanupSDL(app_t *app, bird_t *bird, column_t *column, background_t *background);
    SDL_Texture *loadTexture(char *filename, app_t *app);

    // *** NUEVO: reset "rápido" sin recargar texturas ***
    void game_reset(column_t* pcol, bird_t *bird, menu_t *menu);

    //column_fun.c
    int rand_hole(void);
    void col_mov(column_t* pcol);
  
    //bird_fun.c
    void update_bird_animation(bird_t *bird);
    void bird_jump(bird_t* bird);
    void bird_fall(bird_t* bird);

    //game_logic.c
    char collision(column_t* pcol, bird_t* pbird);
    void colition_update(menu_t* pmenu);
    void points(column_t* pcol, bird_t* pbird, menu_t* menu);
    void score_init(menu_t *pmenu);
    void score_update(menu_t *pmenu, int new_score);
    void score_save(menu_t *pmenu);
    void game_reset(column_t* pcol, bird_t *bird, menu_t *menu);

    //menu.c
    void menu_init(menu_t *menu);
    void menu_set_state(menu_t *menu, int state);
    void menu_next_option(menu_t *menu);
    void menu_prev_option(menu_t *menu);
    void menu_activate_selected(menu_t *menu, column_t *cols, bird_t *bird, app_t *app);

    //input.c
    void window_input(void);

#endif