#include "backend.h"

//Functions

void init(coord_t* pcol, coord_t* pbird){// INicialisation of the var
    srand(time(NULL));

    int aux_x=SPACE; 
    for(int i=0; i<NUM_COL;i++){
        aux_x+=SPACE+COL_WIDTH;  
        pcol[i].x=aux_x;  //Sets the coordinate x of each point of the column
        pcol[i].y=rand_hole(); 

    }
    // Calc of random value with max and min: min + rand() % (max - min + 1);
    pbird->x=3+(rand()%(SPACE/2-2));
    pbird->y= 3+(rand()%(WIDTH/2-2));
    
    
    
}

int rand_hole(void){ //returns a random coord y for the begining of the hole        
    return ((float)rand()/RAND_MAX)*(HEIGHT-HOLE_HEIGHT);
}