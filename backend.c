#include "backend.h"

//Global var from main.c
extern int WIDTH;
extern int HEIGHT;
extern int HOLE_HEIGHT;
extern int NUM_COL;
extern coord_t* column;
extern int COL_WIDTH;
extern int SPACE;
extern float GRAVITY;
extern float JUMP_VEL;

//Functions

void init(coord_t* pcol, coord_t* pbird){

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
    // Calc of random value with max and min: min + rand() % (max - min + 1);
    pbird->x=3+(rand()%(SPACE/2-2));
    pbird->y= 3+(rand()%(WIDTH/2-2));

}

void col_mov(coord_t* pcol){
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
            coord_t next_coord= i==0 ? pcol[NUM_COL-1]: pcol[i-1];
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
    return rand() % (LINES - HOLE_HEIGHT - 1) + 1;
}

//Redefine globals

void set_parameters(void){
    HOLE_HEIGHT = HEIGHT / 3; //Default value
    COL_WIDTH = WIDTH / 15; //Default value
    SPACE = HEIGHT / 2; //Default value
    NUM_COL = WIDTH / COL_WIDTH; //Default value
    GRAVITY = 25.0f; //Default Value
    JUMP_VEL = -8.0f; //Default Value
}

void bird_mov(coord_t bird){
 
}

/*Update screen size fun- NOT WORKING

int update_screen_dimensions() {
    getmaxyx(stdscr, HEIGHT, WIDTH);

    if (HEIGHT < 30 || WIDTH < 30) {
        endwin();
        perror("The screen is too small");
        exit(1);
        return -1;
    }

    int new_col = WIDTH / COL_WIDTH;

    coord_t* pnew_column = realloc(column, new_col * sizeof(coord_t));
    if (pnew_column == NULL) {
        endwin();
        perror("Error al realocar columnas");
        exit(1);
    }

    // Inicializar nuevas columnas si se agrandó
    if(NUM_COL < new_col) {
        for (int i = NUM_COL; i < new_col; i++) {
            pnew_column[i].x = pnew_column[i-1].x + COL_WIDTH + SPACE; // Set outside for new columns
            pnew_column[i].len = COL_WIDTH; // No length initially
            pnew_column[i].y = rand_hole();
        }
    }
    else{
        // If the number of columns decreased, we can keep the existing ones
        for (int i = new_col; i < NUM_COL; i++) {
            pnew_column[i].x = OUTSIDE; // Set outside for removed columns
            pnew_column[i].len = 0; // No length for removed columns
        }
    }
    set_parameters(); // Update parameters based on new dimensions
    column = pnew_column;// Update global pointer

    return 1;
}


void handle_winch(int sig) {
    endwin();
    refresh();
    clear();
    update_screen_dimensions();
}
*/    

    