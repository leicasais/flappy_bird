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

    //macros for the columns
    #define OUTSIDE -1000
    #define COL_PX_H 1024
    #define COL_PX_W 256
    #define BASE_SPEED 2.2f   // velocidad inicial
    #define SPEED_INC 0.06f  // cuánto sube por columna pasada
    #define SPEED_MAX 6.0f  // techo

    //macros birds
    #define HITBOX_X 224
    #define HITBOX_Y 168
    #define NUM_SKINS 5

    //macros for the menus
    #define NUM_OPTIONS_MAIN       3   
    #define NUM_OPTIONS_SKIN       5   
    #define NUM_OPTIONS_GAME_OVER  2
    #define NUM_OPTIONS_PAUSE      4    
    #define MAIN_MENU 1
    #define RUNING 2
    #define PAUSE 3
    #define CONTINUE 4
    #define EXIT 5
    #define RESTART 6
    #define GAME_OVER 7
    #define BEGINING 8
    #define SKIN_MENU 9
    #define NAME_MENU 10  
 
    //macros for the txt's
    #define MAX_SCORES 10
    #define USERNAME_MAX 16


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

    typedef struct{
        int GAME_WIDTH;
        int GAME_HEIGHT;
        int TILE_HIGHT;
        int HOLE_HEIGHT;
        int COL_BOTTOM_WIDTH;
        int COL_TOP_HIGH;            
        int NUM_COL;
        int COL_WIDTH;
        int SPACE;   // px por frame (dinámica               
    }screen_dim_t;

    typedef struct{ //Origin in (1,1)    
        float x;
        float y;
        int len;
        float col_speed;           // px por frame (dinámica)
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
        float h;
        float w;
        float gravity_y;
        float vel_y;
        int scale;
        SDL_Texture *texture;
        SDL_Texture *tex_resurrection;
        int current_frame;
        Uint32 last_frame_time;
    }bird_t;

    typedef struct {
        int score;
        int lives;
        SDL_Texture *full_heart_tex;       //saves the 3 full hearts animation
        SDL_Texture *empty_heart_tex;      //saves the 3 empty heart 
        int heart_h;
        int heart_w;
        SDL_Texture *skins_tex[NUM_SKINS];
        int index_skin;
        int high_score[MAX_SCORES];
        int state;          // MAIN_MENU, RUNING, PAUSE, GAME_OVER, etc.
        int selected;       // índice de opción actualmente seleccionada en el menú activo
        int last_top_pos;
        char username[USERNAME_MAX + 1];
        int  name_editing;   // 0 = no, 1 = yes
    } menu_t;

    /*#########################################
                        Functions
    #########################################*/
    
    //init_fun.c
    void set_parameters(screen_dim_t *screen_dim);
    void init_parameters(screen_dim_t *screen_dim);
    int rand_y_pos(float bird_size, int heart_h,screen_dim_t *screen_dim);
    void init(column_t* pcol, bird_t *bird, menu_t *menu, screen_dim_t *screen_dim);
    void init_tex(column_t* column, bird_t *bird, menu_t *menu, app_t *app, background_t *background, screen_dim_t *screen_dim); 
    void initSDL(app_t *app, screen_dim_t *screen_dim);
    SDL_Texture *loadTexture(char *filename, app_t *app);
    void cleanupSDL(app_t *app, bird_t *bird, column_t *column, background_t *background, menu_t *menu, screen_dim_t *screen_dim);
    void set_bird_skin(bird_t *bird, app_t *app, int idx);

    //column_fun.c
    int rand_hole(screen_dim_t *screen_dim);
    void col_mov(column_t* pcol, screen_dim_t *screen_dim);
    void col_reset_scroll(void);

  
    //bird_fun.c
    void update_bird_animation(bird_t *bird);
    void bird_flying(bird_t *bird);
    void bird_jump(bird_t* bird);
    void bird_fall(bird_t* bird, screen_dim_t *screen_dim);
    void set_bird_skin(bird_t *bird, app_t *app, int idx);


    //game_logic.c
    char collision(column_t* pcol, bird_t* pbird, screen_dim_t *screen_dim);
    void colition_update(menu_t* pmenu);
    void points(column_t* pcol, bird_t* pbird, menu_t* menu, screen_dim_t *screen_dim);
    void history_log(menu_t* pmenu);
    void score_init(menu_t *pmenu);
    int score_update(menu_t *pmenu, int new_score);
    void score_save(menu_t *pmenu);
    void game_reset(column_t* pcol, bird_t *bird, menu_t *menu, screen_dim_t *screen_dim);

    //menu.c
    void menu_init(menu_t *menu);
    void menu_set_state(menu_t *menu, int state);
    void menu_next_option(menu_t *menu);
    void menu_prev_option(menu_t *menu);
    void menu_activate_selected(menu_t *menu, column_t *cols, bird_t *bird, app_t *app, screen_dim_t *screen_dim);

    //input.c
    void window_input(void);

#endif