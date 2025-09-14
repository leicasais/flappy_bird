//std libs and SDL2
#ifndef STRUCTS_H
#define STRUCTS_H
    
    /*#########################################
                        Libraries
    #########################################*/

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
    #include <time.h>
    #include <unistd.h>  

    /*#########################################
                        MACROS
    #########################################*/

    // --- Macros for the main/game flow ---
    #define MS_BTW_FRAMES 16

    // --- Macros for the columns ---
    #define OUTSIDE -1000
    #define COL_PX_H 1024
    #define COL_PX_W 256
    #define BASE_SPEED 2.2f   // initial speed
    #define SPEED_INC 0.08f   // how much it increases per passed column
    #define SPEED_MAX 6.0f    // maximum cap
    #define EASY 0 
    #define MEDIUM 30
    #define EXTREME 60
    #define IMPOSSIBLE 100

    // --- Macros for the birds ---
    #define HITBOX_X 224
    #define HITBOX_Y 168
    #define NUM_SKINS 5

    // --- Macros for the background ---


    // --- Macros for the menus ---
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
 
    // --- Macros for text/scores ---
    #define MAX_SCORES 10
    #define USERNAME_MAX 16

    // --- Panel layout (tweaks) ---
    #define PANEL_W_RATIO     0.58f   
    #define PANEL_H_RATIO     0.78f   
    #define PANEL_BOTTOM_PAD  52      
    #define OPTION_LINE_H        32    
    #define OPTION_GAP           16    
    #define OPTION_HILIGHT_PAD_Y 8     
        
    #define LIST_OPTIONS_GAP  18   
    #define HILIGHT_INSET     20    

    /*#########################################
                        Structs
    #########################################*/
    typedef struct {
        float x;          // in case you use a “real” camera on X/Y
        float y;
        char   shaking;   // 1 if it is shaking
        Uint32 start_ms;  // shake start time
        float duration;   // ms
        float amp;        // amplitude in pixels
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
        SDL_Texture * clouds;
    }background_t;

    typedef struct {
        SDL_Texture **frames;   // array of textures [count]
        int count;              // number of frames
        float frame_time;       // ms per frame (e.g. 30 ms)
        int playing;            // 1 if animation is active
        int cur;                // current frame index
        float acc_ms;           // time accumulator to advance frames
        int x, y;               // screen position (center)
        int w, h;               // destination size (0 = use frame size)
    } ExplotionAnim;

    typedef struct{
        int GAME_WIDTH;
        int GAME_HEIGHT;
        int TILE_HIGHT;
        int HOLE_HEIGHT;
        int COL_BOTTOM_WIDTH;
        int COL_TOP_HIGH;            
        int NUM_COL;
        int COL_WIDTH;
        int SPACE;   // px per frame (dynamic)               
    }screen_dim_t;

    typedef struct{ // Origin at (1,1)    
        float x;
        float y;
        int len;
        float col_speed;           // px per frame (dynamic)
        float col_speed_y;
        SDL_Texture *texture_down;
        SDL_Texture *texture_up;
        int trim;
        int current_frame;
        Uint32 last_frame_time;
    }column_t;

    typedef struct{
        float y_top;
        float y_bottom; 
        float x_l;
        float x_r;
        float h;
        float w;
        float gravity_y;
        float vel_y;
        float scale;
        SDL_Texture *texture;
        SDL_Texture *tex_resurrection;
        int current_frame;
        Uint32 last_frame_time;
    }bird_t;

    typedef struct {
        int score;
        int lives;
        SDL_Texture *full_heart_tex;       // stores the 3 full hearts
        SDL_Texture *empty_heart_tex;      // stores the 3 empty hearts
        int heart_h;
        int heart_w;
        SDL_Texture *skins_tex[NUM_SKINS];
        int index_skin;
        int high_score[MAX_SCORES];
        int state;          // MAIN_MENU, RUNING, PAUSE, GAME_OVER, etc.
        int selected;       // index of the currently selected option in the active menu
        int last_top_pos;
        char username[USERNAME_MAX + 1];
        int  name_editing;  // 0 = no, 1 = yes
        int dificulty;
    } menu_t;

#endif