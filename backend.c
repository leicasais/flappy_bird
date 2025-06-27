#include "backend.h"

//Functions

void init(coord_t* pcol, coord_t* pbird){

    int aux_x=0; 
    for(int i=0; i<NUM_COL;i++){
        aux_x+=SPACE+COL_WIDTH;  
        pcol[i].x=aux_x;  //Sets the coordinate x of each point of the column
        pcol[i].y=rand_hole(); 

    }
    // Calc of random value with max and min: min + rand() % (max - min + 1);
    pbird->x=3+(rand()%(SPACE/2-2));
    pbird->y= 3+(rand()%(WIDTH/2-2));
    
    
    
}

void col_mov(coord_t* pcol){
    int cant_c;
    for(int i=0; i<NUM_COL; i++){
        cant_c=lines_col(pcol[i]);
        if(pcol[i].x > 1){
            pcol[i].x-=1;
        }
        else{//0 lines in the next column
            pcol[i].x= WIDTH-1;
            pcol[i].y= rand_hole();
        }
    }
}

int lines_col(coord_t pcol){ 
    
    if(pcol.x<1 || pcol.x>(WIDTH+1)){
        return -1;//Error, invalid coord
    }
    if((pcol.x>COL_WIDTH) && (pcol.x<(WIDTH-COL_WIDTH))){ //they are compleated
        return COL_WIDTH;
    }
    else{
        if(pcol.x<=COL_WIDTH){
            return pcol.x;
        }
        else{
            return WIDTH-pcol.x;
        }
    }
    
}



int rand_hole(void){ //returns a random coord y for the begining of the hole        
    return rand() % (HEIGHT - HOLE_HEIGHT);;
}