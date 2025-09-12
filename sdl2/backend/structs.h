//std libs and SDL2
#ifndef STRUCTS_H
#define STRUCTS_H
    
    /*#########################################
                        Librerias
    #########################################*/

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
    #include <time.h>
    #include <unistd.h>  // usleep()

    /*#########################################
                        MACROS
    #########################################*/

    // --- Macros for the main/game flow ---
    #define MS_BTW_FRAMES 16

    // --- macros for the columns ---
    #define OUTSIDE -1000
    #define COL_PX_H 1024
    #define COL_PX_W 256
    #define BASE_SPEED 2.2f   // velocidad inicial
    #define SPEED_INC 0.08f  // cuánto sube por columna pasada
    #define SPEED_MAX 6.0f  // techo
    #define EASY 0 
    #define MEDIUM 30
    #define EXTREME 60
    #define IMPOSSIBLE 100

    // --- macros birds ---
    #define HITBOX_X 224
    #define HITBOX_Y 168
    #define NUM_SKINS 5

    // --- macros for the menus ---
    #define NUM_OPTIONS_MAIN       4   
    #define NUM_OPTIONS_SKIN       5   
    #define NUM_OPTIONS_DIFICULTY  4
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
    #define DIFICULTY_MENU 11  
 
    // --- macros for the txt's ---
    #define MAX_SCORES 10
    #define USERNAME_MAX 16

    // --- Layout del panel (ajustes) ---
    #define PANEL_W_RATIO     0.58f   
    #define PANEL_H_RATIO     0.78f   
    #define PANEL_BOTTOM_PAD  52      
    #define OPTION_LINE_H        32    
    #define OPTION_GAP           16    
    #define OPTION_HILIGHT_PAD_Y 8     
        
    #define LIST_OPTIONS_GAP  18   // espacio fijo entre la lista y las opciones
    #define HILIGHT_INSET     20      // sangría del destacador

    /*#########################################
                        Structs
    #########################################*/

    typedef struct {
        float x;          // por si tenés cámara “real” en X/Y
        float y;
        char   shaking;    // 1 if it is shaking
        Uint32 start_ms;  // inicio del shake
        float duration;   // ms
        float amp;        // amplitud en píxeles
        float freq;       // Hz
    } camera_t;

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
        float col_speed_y;
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
        int dificulty;
    } menu_t;

#endif