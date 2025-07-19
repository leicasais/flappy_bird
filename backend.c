#include "backend.h"

//Global var from main.c
extern int WIDTH;
extern int HEIGHT;
extern int HOLE_HEIGHT;
extern int NUM_COL;
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;
extern float GRAVITY;
extern float JUMP_VEL;

//Inicialisations and parameters functions

void set_parameters(void){          //Redefine globals
    HOLE_HEIGHT = HEIGHT / 3;       //Default value
    COL_WIDTH = WIDTH / 15;         //Default value
    SPACE = HEIGHT / 2;             //Default value
    NUM_COL = WIDTH / COL_WIDTH;    //Default value
    GRAVITY = 0.25f;                 //Default Value
    JUMP_VEL = -0.05f;               //Default Value
}

void init(column_t* pcol, bird_t *bird, menu_t *menu){
//Column init
    int aux_x=0; 
    int i;
    for(i=0; i<(WIDTH/(COL_WIDTH+SPACE));i++){
        aux_x+=SPACE+COL_WIDTH;  
        pcol[i].x=aux_x;  //Sets the coordinate x of each point of the column
        pcol[i].y=rand_hole();
        pcol[i].len=COL_WIDTH; //Sets the length of the column
    }
    for(int j=i;j<NUM_COL;j++){ //col outside the screen prepearing to enter
        pcol[j].x=OUTSIDE;
        pcol[j].len=0;
    }
//Bird init
    bird->x= SPACE/2;
    bird->y= 3+(rand()%(HEIGHT/2-2));//inicialite the position bird
    bird->vel_y = 0;
//Menue init
    menu->lives=3;
    menu->score=0;
    menu->state= MAIN_MENU;
}

//Column funcions

void col_mov(column_t* pcol){
    for(int i=0; i<NUM_COL; i++){
        if(pcol[i].len){
            (pcol[i].x)--;
            if(pcol[i].x < COL_WIDTH && pcol[i].x>=0){
                (pcol[i].len)--; //Update the length of the column
            }
            else if(pcol[i].x<WIDTH && pcol[i].x>(WIDTH-COL_WIDTH)){
                (pcol[i].len)++; //Update the length of the column
            }
        }
        else{//0 lines in the next column
            column_t next_coord= i==0 ? pcol[NUM_COL-1]: pcol[i-1];
            if(next_coord.x==OUTSIDE || (((next_coord.len)+next_coord.x) >= WIDTH-SPACE) ){
                pcol[i].x=OUTSIDE;
            }
            else{
                pcol[i].x= WIDTH-1;
                pcol[i].y= rand_hole();
                pcol[i].len=1; //Reset the length of the column
            }
        }
    }
}


int rand_hole(void){ //returns a random coord y for the begining of the hole        
    return rand() % (HEIGHT - HOLE_HEIGHT - 1) + 1;
}


//Bird funcions
void bird_mov(bird_t* bird){
    bird->y     += bird->vel_y;
    bird->vel_y += GRAVITY;

    // Don't allow to the bird go so low
    if (bird->y > HEIGHT) {
        bird->y = HEIGHT;
        bird->vel_y = 0;
    }
    // Don't allow to the bird go so high
    if (bird->y < 0) {
        bird->y = 0;
        bird->vel_y = 0;
    }
}
// Detect a Jump and change de bird velocity
void bird_jump(bird_t* bird) {
    bird->vel_y = JUMP_VEL;
}


char collision(column_t* pcol, bird_t* pbird){
    column_t intersection_col = {.x=OUTSIDE};

    for (int i=0; i<NUM_COL;i++){
        if((pbird->x >= pcol[i].x ) && (pbird->x < (pcol[i].x + COL_WIDTH))){//Gives the column that is in the corrd x of the bird 
            intersection_col=pcol[i];
        }
    }
    if(intersection_col.x != OUTSIDE && (!((pbird->y >= intersection_col.y) && (pbird->y < (intersection_col.y + HOLE_HEIGHT))))){ //if there is a column that has the same coord x as the bird and the coord y is not the same as the ones in the hole
        return 1; //the bird collides
    }
    else{
        return 0;
    }
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
                    case 0: menu->state= RUNING;break;
                    case 1: menu->state= EXIT; break;
                }
    }
}

void data_track(menu_t* pmenu){     // Updates game statistics such as score and lives.

} 
    