#include "backend.h"

//Global var from main.c
extern int GAME_WIDTH;
extern int GAME_HEIGHT;
extern int HOLE_HEIGHT;
extern int COL_BOTTOM_WIDTH;
extern int COL_TOP_HIGH;            
extern int NUM_COL;
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;
extern int BIRD_SCALE;


//Column funcions

void col_mov(column_t* pcol){
    for(int i=0; i<NUM_COL; i++){
        if(pcol[i].len){
            (pcol[i].x)--;
            if(pcol[i].x < 0){//Case the column is leaving the screen 
                (pcol[i].len)--; //Update the length of the column
                (pcol[i].x)=0;
            }
            else if(pcol[i].len>=1 && pcol[i].len<=COL_WIDTH){//Case the column is entering the screen from the right
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
    return (COL_TOP_HIGH - 1)+ rand() % (GAME_HEIGHT -2*COL_TOP_HIGH + 1);
}
