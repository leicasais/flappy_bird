#include "backend.h"


//Inicialisations and parameters functions

void set_parameters(screen_dim_t *screen_dim){          //Redefine globals dependidng on the screen size 
    screen_dim-> HOLE_HEIGHT = (screen_dim->GAME_HEIGHT) / 3;       
    screen_dim->COL_WIDTH = screen_dim->GAME_WIDTH / 10;         
    screen_dim->SPACE = screen_dim->GAME_HEIGHT/3;             
    screen_dim->NUM_COL = screen_dim->GAME_WIDTH / screen_dim->COL_WIDTH;    
    screen_dim->TILE_HIGHT = screen_dim->GAME_HEIGHT /9;
}

void init_parameters(screen_dim_t *screen_dim){
    //Screen init
    screen_dim->GAME_HEIGHT= 720;
    screen_dim->GAME_WIDTH= 1280;
    set_parameters(screen_dim);
}

int rand_y_pos(float bird_size, int heart_h,screen_dim_t *screen_dim){ //returns a random coord y for the begining of the hole  OBS-> min + rand() % (max - min + 1);      
    const int margin_inf = 3;  // lo que quieras dejar libre arriba
    int minY = heart_h;
    int maxY = (screen_dim->GAME_HEIGHT) - (screen_dim->TILE_HIGHT) - bird_size - margin_inf - 1; // límite correcto

    if (maxY < minY) {
        // agujero imposible de colocar: ajustá HOLE_HEIGHT o márgenes
        maxY = minY;
    }
    return minY + rand() % (maxY - minY + 1); // rango [minY, maxY]
}

void init(column_t* pcol, bird_t *bird, menu_t *menu, screen_dim_t *screen_dim){  
//Column init
    int aux_x=0; 
    int i;
    for(i=0; i<((screen_dim->GAME_WIDTH/(screen_dim->COL_WIDTH + screen_dim->SPACE)) -1);i++){
        aux_x+=screen_dim->SPACE+screen_dim->COL_WIDTH;  
        pcol[i].x=aux_x;  //Sets the coordinate x of each point of the column
        pcol[i].y=rand_hole(screen_dim);
        pcol[i].len=screen_dim->COL_WIDTH;
    }
    if(screen_dim->GAME_WIDTH % (screen_dim->COL_WIDTH + screen_dim->SPACE)){//if GAME_WIDTH/(COL_WIDTH+SPACE) was supposed to be a float -> we have to put a 'weird case'
        int space_left=screen_dim->GAME_WIDTH-(screen_dim->COL_WIDTH+screen_dim->SPACE)*(i+1);//The space left after saving the last column+space in the for
        pcol[i].x=aux_x+screen_dim->SPACE+screen_dim->COL_WIDTH;
        pcol[i].y=rand_hole(screen_dim);
        if(space_left<screen_dim->COL_WIDTH){ //case the last column of the screen is not shown fully 
            pcol[i].len=space_left;
        }
        else{//Case the column is shown fully but there is not SPACE between the last column and the edge of the screen
            pcol[i].len=screen_dim->COL_WIDTH;
        }
        i++;
    }
    for(int j=i;j<screen_dim->NUM_COL;j++){ //col outside the screen prepearing to enter
        pcol[j].x=OUTSIDE;
        pcol[j].len=0;
    }
    pcol->col_speed=BASE_SPEED;   // px por frame (dinámica)
    

    //Bird init
    bird->scale =3;
    bird->h =(HITBOX_Y)/(bird->scale);
    bird->w =(HITBOX_X)/(bird->scale);

    bird->x_l= screen_dim->SPACE/2 + ( (bird->w) /2);
    bird->x_r= bird->x_l+(bird->w);

    float y= rand_y_pos(bird->h, menu->heart_h, screen_dim);
    bird->gravity_y = 0.4;//Despues esta opcion depende el menu pero por ahora lo dejo como si siempre estuviese en la tierra
    bird->y_top=y;
    bird->y_bottom=bird->y_top+(bird->h);

    //Menue init
    menu_init(menu); 
}

void init_tex(column_t* column, bird_t *bird, menu_t *menu, app_t *app, background_t *background, screen_dim_t *screen_dim){
       //column textures
    for(int i=0; i<screen_dim->NUM_COL; i++){
        column[i].texture_down = loadTexture("../img/columns/Col.png", app);
        column[i].texture_up = loadTexture("../img/columns/Col.png", app);
        column[i].trim=0;
    }

        //Bakground textures 
    background->tile_tex = loadTexture("../img/background/Tile.png", app);

        //bird textures
    (menu->skins_tex)[0]= loadTexture("../img/birds/Purple_bird.png", app);
    (menu->skins_tex)[1]= loadTexture("../img/birds/Brain_bird.png", app);
    (menu->skins_tex)[2]= loadTexture("../img/birds/Yellow_bird.png", app);
    (menu->skins_tex)[3]= loadTexture("../img/birds/Future_bird.png", app);
    (menu->skins_tex)[4]= loadTexture("../img/birds/Angry_bird.png", app);

    bird->texture= (menu->skins_tex)[0]; //monto cambia el inide fijo por menu->index_skin que se puede seleccionar en tu menu
    bird->tex_resurrection = loadTexture("../img/birds/Resurecting_bird.png", app);
    bird->current_frame=0;
    bird->last_frame_time = SDL_GetTicks();

        //hearts text initmenu->full_heart_tex = loadTexture("../img/simbols/Full_heart.png", app);
    menu->full_heart_tex = loadTexture("../img/simbols/Full_heart.png", app);
    menu->empty_heart_tex = loadTexture("../img/simbols/Empty_heart.png", app);

        //points letters init
    app->font = TTF_OpenFont("../img/fonts/Jersey15-Regular.ttf", 26); // tamaño “físico” en px
    if (!app->font) {
        SDL_Log("OpenFont FAIL: %s", TTF_GetError());
        exit(1);
    }
    app->score_tex = NULL;
    app->score_w = app->score_h = 0;
    app->score_color = (SDL_Color){ 20, 20, 20, 255 }; // gris oscuro
}

void initSDL(app_t *app, screen_dim_t *screen_dim){
    //init windows
    int rendererFlags, windowFlags;
    rendererFlags = SDL_RENDERER_ACCELERATED;   //tells SDL to use hardware acceleration for the renderer (faster graphics performance via GPU).
    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app->window = SDL_CreateWindow("Floppy bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_dim->GAME_WIDTH, screen_dim->GAME_HEIGHT, windowFlags);       //SDL_WINDOWPOS_UNDEFINED tells SDL to let the OS position the window wherever it likes

    if (!(app->window)){
        printf("Failed to open %d x %d window: %s\n", screen_dim->GAME_WIDTH, screen_dim->GAME_HEIGHT, SDL_GetError());
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
void cleanupSDL(app_t *app, bird_t *bird, column_t *column, background_t *background, menu_t *menu, screen_dim_t *screen_dim){
    // 1) Texturas y recursos dependientes del renderer
    if (app->score_tex) { 
        SDL_DestroyTexture(app->score_tex); 
        app->score_tex = NULL;
    }
    for(int i=0; i< NUM_SKINS;i++){
        if((menu->skins_tex)[i]){
            SDL_DestroyTexture((menu->skins_tex)[i]);
            (menu->skins_tex)[i] = NULL;
        }
    }
    if (bird->texture) { 
        SDL_DestroyTexture(bird->texture); 
        bird->texture = NULL; 
    }
    if (bird->tex_resurrection) { 
        SDL_DestroyTexture(bird->tex_resurrection); 
        bird->tex_resurrection = NULL; 
    }
    if (background->tile_tex) { 
        SDL_DestroyTexture(background->tile_tex); 
        background->tile_tex = NULL; 
    }
    for (int i = 0; i < screen_dim->NUM_COL; i++){
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
    if(menu->full_heart_tex){
        SDL_DestroyTexture(menu->full_heart_tex);
        menu->full_heart_tex = NULL; 
    }
    if(menu->empty_heart_tex){
        SDL_DestroyTexture(menu->empty_heart_tex);
        menu->empty_heart_tex = NULL; 
    }


    // 2) renderer y ventana
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

void set_bird_skin(bird_t *bird, app_t *app, int idx)
{
    // Paths de las 5 skins
    static const char *SKINS[5] = {
        "../img/birds/Angry_bird.png",
        "../img/birds/Brain_bird.png",
        "../img/birds/Future_bird.png",
        "../img/birds/Purple_bird.png",
        "../img/birds/Yellow_bird.png"
    };

    if (idx < 0 || idx >= 5){
        return;
    }
    SDL_Texture *newtex = loadTexture((char*)SKINS[idx], app);
    if (!newtex) {
        return;
    }

    if (bird->texture) {
        SDL_DestroyTexture(bird->texture);
    }
    bird->texture = newtex;
}


