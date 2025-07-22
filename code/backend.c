#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int HOLE_HEIGHT;
extern int NUM_COL;
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;

//Inicialisations and parameters functions

void set_parameters(void){          //Redefine globals
    HOLE_HEIGHT = GAME_HEIGHT / 3;       //Default value
    COL_WIDTH = GAME_WIDTH / 15;         //Default value
    SPACE = GAME_HEIGHT;             //Default value
    NUM_COL = GAME_WIDTH / COL_WIDTH;    //Default value
}

void init(column_t* pcol, bird_t *bird, menu_t *menu){
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

//Bird init
    bird->x= SPACE/2;
    bird->y= 3+(rand()%(GAME_HEIGHT/2-2));//inicialite the position bird
    bird->gravity_y = 0.004;//Despues esta opcion depende el menu pero por ahora lo dejo como si siempre estuviese en la tierra
    bird->y_top=(int)(bird->y);
    bird->y_bottom=bird->y_top+1;
    //Menue init
    menu->lives=3;
    menu->score=0;
}


//Column funcions

void col_mov(column_t* pcol){
    for(int i=0; i<NUM_COL; i++){
        if(pcol[i].len){
            (pcol[i].x)--;
            if(pcol[i].x < 0){//Case the column is leaving the screen 
                (pcol[i].len)--; //Update the length of the column
                (pcol[i].x)=0;
            }
            else if(pcol[i].len>=1 && pcol[i].len<COL_WIDTH){//Case the column is entering the screen from the right
                (pcol[i].len)++; //Update the length of the column
            }
        }
        else{//0 lines in the next column
            column_t next_coord= i==0 ? pcol[NUM_COL-1]: pcol[i-1];
            if(next_coord.x==OUTSIDE || ((((next_coord.len)+next_coord.x + SPACE) >= (GAME_WIDTH-1)) && i) || ((((next_coord.len)+next_coord.x + SPACE) > (GAME_WIDTH-1)) && !i)){//We have to considerate that if i=0 the new values for the new frame for next_coord are not yet updated, and if i!=0 then those values are updated
                pcol[i].x=OUTSIDE;
            }
            else{
                pcol[i].x= GAME_WIDTH-1;
                pcol[i].y= rand_hole();
                pcol[i].len=1; //Reset the length of the column
            }
        }
    }
}


int rand_hole(void){ //returns a random coord y for the begining of the hole        
    return rand() % (GAME_HEIGHT - HOLE_HEIGHT - 1) + 1;
}


//Bird funcions
void bird_mov(bird_t* bird, int ch){
    float jump_displacement=-0.1; //how far the bird would jump without gravity
    if(ch == ' '){
        bird->delta_y= jump_displacement;
    }
    bird->delta_y +=bird->gravity_y;
    bird->y+=bird->delta_y;

    // Don't allow the bird going out of the screen 
    if (bird->y > (GAME_HEIGHT-1)) {
        bird->y = GAME_HEIGHT-1;
        bird->delta_y=0;
    }
    else if (bird->y < 1) {
        bird->y = 1;
        bird->delta_y =0;
    }
    //Update hitbox
    bird->y_top=(int)(bird->y);
    bird->y_bottom=bird->y_top+1;
    
}

char collision(column_t* pcol, bird_t* pbird){
    for (int i = 0; i < NUM_COL; i++) {
        if (pcol[i].len == 0) continue; // Saltar columnas no activas

        int col_left = pcol[i].x;
        int col_right = col_left + pcol[i].len;
        int hole_top = pcol[i].y;
        int hole_bottom = hole_top + HOLE_HEIGHT;

        if ((pbird->x >= col_left) && (pbird->x < col_right)) {
            // Verificar si el pájaro NO está dentro del hueco
            if (pbird->y_bottom <= hole_top || pbird->y_top >= hole_bottom) {
                return 1;
            }
        }
    }
    return 0;
}

//Menue funtions

void main_menu(int key, menu_t *menu, int *selection){
    
    switch (key) {
            case KEY_UP:
                (*selection)--;
                if((*selection)<0){
                    (*selection)=(NUM_OPTIONS_MAIN-1);
                }
                break;
            case KEY_DOWN:
                (*selection)++;
                if((*selection)==NUM_OPTIONS_MAIN){
                    *selection=0;
                }
                break;
            case '\n':
                switch (*selection) {
                    case 0: menu->state= RESTART;
                    break;
                    case 1: menu->state= EXIT; 
                    break;
                }
    }
}
void pause_menu(int key, menu_t *menu, int *selection){
    
    switch (key) {
            case KEY_UP:
                (*selection)--;
                if((*selection)<0){
                    (*selection)=(NUM_OPTIONS_PAUSE-1);
                }
                break;
            case KEY_DOWN:
                (*selection)++;
                if((*selection)==NUM_OPTIONS_PAUSE){
                    *selection=0;
                }
                break;
            case '\n':
                switch (*selection) {
                    case 0: menu->state= RUNING;
                    break;
                    case 1: menu->state= RESTART; 
                    break;
                    case 2: menu->state= MAIN_MENU;
                    break;
                    case 3: menu->state= EXIT;
                    break;
                }
    }
}

void game_over_menu(int key, menu_t *menu, int *selection){
    switch (key) {
            case KEY_UP:
                (*selection)--;
                if((*selection)<0){
                    (*selection)=(NUM_OPTIONS_GAME_OVER-1);
                }
                break;
            case KEY_DOWN:
                (*selection)++;
                if((*selection)==NUM_OPTIONS_GAME_OVER){
                    *selection=0;
                }
                break;
            case '\n':
                switch (*selection) {
                    case 0: menu->state= RESTART;
                    break;
                    case 1: menu->state= MAIN_MENU; 
                    break;
                    case 2: menu->state= EXIT;
                    break;
                }
    }
}

void colition_update(menu_t* pmenu){     // Updates game statistics such as score and lives.
    pmenu->lives --;
    if(pmenu->lives < 0){
        pmenu->state = GAME_OVER;
    }
} 
    