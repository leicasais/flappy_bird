#include "frontend.h"
#include "backend.h"

//Function
void display_col(coord_t* pcol){
    for(int i=0; i< NUM_COL; i++){
        for(int j=0;j<lines_col(pcol[i]);j++){
            for(int k=1; k< HEIGHT;k++){
                if( k == (pcol[i].y) ){
                    k+=HOLE_HEIGHT;
                }
                mvaddch(k,(pcol[i].x)+j,'*');
            }
            
        }  
        mvprintw(0, 0, "Columna 0: ancho = %d", lines_col(pcol[i])); 
    }
}
