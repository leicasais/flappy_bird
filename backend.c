#include "backend.h"

//Global var from main.c
extern int WIDTH;
extern int HEIGHT;
extern int HOLE_HEIGHT;
extern int NUM_COL;
extern coord_t* column;
extern int COL_WIDTH;
extern int SPACE;


//Functions

void init(coord_t* pcol, coord_t* pbird){

    int aux_x=SPACE; 
    for(int i=0; i<NUM_COL;i++){
        pcol[i].x=aux_x;  //Sets the coordinate x of each point of the column
        aux_x+=SPACE+COL_WIDTH;  
        pcol[i].y=rand_hole(); 

    }
    // Calc of random value with max and min: min + rand() % (max - min + 1);
    pbird->x=3+(rand()%(SPACE/2-2));
    pbird->y= 3+(rand()%(WIDTH/2-2));
    
    
    
}

void col_mov(coord_t* pcol){
    for(int i=0; i<NUM_COL; i++){
        if(pcol[i].x > 1){
            pcol[i].x-=1; 
        }
        else{//0 lines in the next column
            pcol[i].x= WIDTH -1;
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
    return rand() % (LINES - HOLE_HEIGHT - 1) + 1;;
}

/*Update screen size fun- NOT WORKING

int update_screen_dimensions() {
    getmaxyx(stdscr, HEIGHT, WIDTH);

    if (HEIGHT < 30 || WIDTH < 30) {
        return -1;
    }

    int new_col = WIDTH / (COL_WIDTH + SPACE);

    coord_t* new_column = realloc(column, new_col * sizeof(coord_t));
    if (new_column == NULL) {
        endwin();
        perror("Error al realocar columnas");
        exit(1);
    }
    NUM_COL = new_col;
    HOLE_HEIGHT = HEIGHT / 3;

    // Inicializar nuevas columnas si se agrandó
    for (int i = NUM_COL; i < new_col; i++) {
        new_column[i].x = WIDTH -1;
        new_column[i].y = rand_hole();
    }
    column = new_column;

    return 1;
}


void handle_winch(int sig) {
    endwin();
    refresh();
    clear();
    update_screen_dimensions();
}
*/