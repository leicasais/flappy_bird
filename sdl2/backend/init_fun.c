#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int TILE_HIGHT;
extern int HOLE_HEIGHT;        
extern int NUM_COL;
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;


//Inicialisations and parameters functions

void set_parameters(void){          //Redefine globals dependidng on the screen size 
    HOLE_HEIGHT = GAME_HEIGHT / 3;       
    COL_WIDTH = GAME_WIDTH / 10;         
    SPACE = GAME_HEIGHT/3;             
    NUM_COL = GAME_WIDTH / COL_WIDTH;    
    TILE_HIGHT = GAME_HEIGHT /9;
}

void init_parameters(void){
    //Screen init
    GAME_HEIGHT= 720;
    GAME_WIDTH= 1280;
    set_parameters();
}

void init(column_t* pcol, bird_t *bird, menu_t *menu, app_t *app, background_t *background){  
//Column init
    int aux_x=0; 
    int i;
    for(i=0; i<((GAME_WIDTH/(COL_WIDTH+SPACE)) -1);i++){
        aux_x+=SPACE+COL_WIDTH;  
        pcol[i].x=aux_x;  //Sets the coordinate x of each point of the column
        pcol[i].y=rand_hole();
        pcol[i].len=COL_WIDTH; //Sets the length of the column
    }
    if(GAME_WIDTH%(COL_WIDTH+SPACE)){//if GAME_WIDTH/(COL_WIDTH+SPACE) was supposed to be a float -> we have to put a 'weird case'
        int space_left=GAME_WIDTH-(COL_WIDTH+SPACE)*(i+1);//The space left after saving the last column+space in the for
        pcol[i].x=aux_x+SPACE+COL_WIDTH;
        pcol[i].y=rand_hole();
        if(space_left<COL_WIDTH){ //case the last column of the screen is not shown fully 
            pcol[i].len=space_left;
        }
        else{//Case the column is shown fully but there is not SPACE between the last column and the edge of the screen
            pcol[i].len=COL_WIDTH;
        }
        i++;
    }
    for(int j=i;j<NUM_COL;j++){ //col outside the screen prepearing to enter
        pcol[j].x=OUTSIDE;
        pcol[j].len=0;
    }
    //column textures
    for(int i=0; i<NUM_COL; i++){
        column[i].texture_down = loadTexture("../img/columns/Col.png", app);
        column[i].texture_up = loadTexture("../img/columns/Col.png", app);
        column[i].trim=0;
    }

    //Bakground textures 
    background->tile_tex = loadTexture("../img/background/Tile.png", app);

    //Bird init
    bird->scale =3;
    bird->x_l= SPACE/2 + ( (HITBOX_X*(bird->scale)) /2);
    bird->x_r= bird->x_l+(HITBOX_X*(bird->scale));

    float y= 3+(rand()%((GAME_HEIGHT-TILE_HIGHT)-(3+((HITBOX_Y*(bird->scale))/2))));//inicialite the position bird
    bird->gravity_y = 0.4;//Despues esta opcion depende el menu pero por ahora lo dejo como si siempre estuviese en la tierra
    bird->y_top=(int)y;
    bird->y_bottom=bird->y_top+(HITBOX_Y*(bird->scale));

    //bird texture
    bird->texture= loadTexture("../img/birds/Player_Rainbow.png", app);
    bird->current_frame=0;
    bird->last_frame_time = SDL_GetTicks();

    //points letters init
    app->font = TTF_OpenFont("../img/fonts/Jersey15-Regular.ttf", 16); // tamaño “físico” en px
    if (!app->font) {
        SDL_Log("OpenFont FAIL: %s", TTF_GetError());
        exit(1);
    }

    app->score_tex = NULL;
    app->score_w = app->score_h = 0;
    app->score_color = (SDL_Color){ 20, 20, 20, 255 }; // gris oscuro

    //Menue init
    menu->lives=3;
    menu->score=0;
}

void initSDL(app_t *app){
    //init windows
    int rendererFlags, windowFlags;
    rendererFlags = SDL_RENDERER_ACCELERATED;   //tells SDL to use hardware acceleration for the renderer (faster graphics performance via GPU).
    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app->window = SDL_CreateWindow("Floppy bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_WIDTH, GAME_HEIGHT, windowFlags);       //SDL_WINDOWPOS_UNDEFINED tells SDL to let the OS position the window wherever it likes

    if (!(app->window)){
        printf("Failed to open %d x %d window: %s\n", GAME_WIDTH, GAME_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");       //"linear" enables smoother scaling of images/textures (anti-aliasing), resize graphics and want them to look less pixelated

    app->renderer = SDL_CreateRenderer(app->window, -1, rendererFlags);       //Attached to app.window, -1: SDL picks the best rendering driver

    if (!(app->renderer)){
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    //init img      
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;     //ctivates the ability to load .png and .jpg files
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
        printf("Failed to init SDL_image: %s\n", IMG_GetError());
        exit(1);    
    }
    //init txt
    if (TTF_Init() != 0) {
        SDL_Log("TTF_Init FAIL: %s", TTF_GetError());
        exit(1);
    }

}

SDL_Texture* loadTexture(char *filename, app_t *app){
    SDL_Texture *texture;               //A texture in SDL is an image loaded into GPU memory that can be drawn on the screen using SDL_RenderCopy()
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);        //for debugging purposes, to let you know which file is being loaded
    texture = IMG_LoadTexture(app->renderer, filename);      // loads the image file (JPG, PNG, etc.) from filename.It converts it into a SDL_Texture using your app.renderer.
    
    if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0) {   // mode so the game doent have a “blur” in pixel art:
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SetBlend FAIL %s: %s", filename, SDL_GetError());
    }
    return texture;
}

//exit fun
void cleanupSDL(app_t *app, bird_t *bird, column_t *column, background_t *background){
    // 1) Texturas y recursos dependientes del renderer
    if (app->score_tex) { 
        SDL_DestroyTexture(app->score_tex); 
        app->score_tex = NULL;
    }
    if (bird->texture) { 
        SDL_DestroyTexture(bird->texture); 
        bird->texture = NULL; 
    }
    if (background->tile_tex) { 
        SDL_DestroyTexture(background->tile_tex); 
        background->tile_tex = NULL; 
    }
    for (int i = 0; i < NUM_COL; i++){
        if (column[i].texture_down) { 
            SDL_DestroyTexture(column[i].texture_down); 
            column[i].texture_down = NULL; 
        }
        if (column[i].texture_up) { 
            SDL_DestroyTexture(column[i].texture_up);   
            column[i].texture_up   = NULL; 
        }
    }
    if (app->font) { 
        TTF_CloseFont(app->font); 
        app->font = NULL; 
    }

    // 2) Ahora sí: renderer y ventana
    if (app->renderer) { 
        SDL_DestroyRenderer(app->renderer); 
        app->renderer = NULL; 
    }
    if (app->window) { 
        SDL_DestroyWindow(app->window);     
        app->window   = NULL; 
    }

    // 3) Cierres de libs
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

