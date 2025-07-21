#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int HOLE_HEIGHT;
extern int NUM_COL;
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;
extern float GRAVITY;
extern float JUMP_VEL;

//Inicialisations and parameters functions

void set_parameters(void){          //Redefine globals
    HOLE_HEIGHT = GAME_HEIGHT / 3;       //Default value
    COL_WIDTH = GAME_WIDTH / 15;         //Default value
    SPACE = GAME_HEIGHT ;             //Default value
    NUM_COL = GAME_WIDTH / COL_WIDTH;    //Default value
}

void init(column_t* pcol){
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


