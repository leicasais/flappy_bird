#include "backend.h"

//Global var from main.c
extern int WIDTH;
extern int HEIGHT;
extern int HOLE_HEIGHT;
extern int NUM_COL; // ALL the columns in the ar counting the ones that are OUTSIDE
extern column_t* column;
extern int COL_WIDTH;
extern int SPACE;


//Functions

void init(column_t* pcol){

    int aux_x=0; 
    int i;
    for(i=0; i<(WIDTH/(COL_WIDTH+SPACE));i++){
        aux_x+=SPACE+COL_WIDTH;  
        pcol[i].x=aux_x;  //Sets the coordinate x of each point of the column
        pcol[i].y=rand_hole();
        pcol[i].len=COL_WIDTH; //Sets the length of the column
        pcol[i].num_col=i+1;
    }
    for(int j=i;j<NUM_COL;j++){ //col outside the screen prepearing to enter
        pcol[j].x=OUTSIDE;
        pcol[j].len=0;
        pcol[j].num_col=j+1;
    }

}

void col_mov(column_t* pcol){
    for(int i=0; i<NUM_COL; i++){
        if(pcol[i].len){
            (pcol[i].x)--;
            if(pcol[i].x < COL_WIDTH && pcol[i].x>=0){
                (pcol[i].len)--; //Case  -> col is disappearing from the screen so x is smaller than the COL_WIDTH
            }
            else if(pcol[i].x<WIDTH && pcol[i].x>(WIDTH-COL_WIDTH)){
                (pcol[i].len)++; //Case -> col is apearing from the right corner of the screen
            }
        }
        else{//0 lines in the next column are shown on the screen or 0 lines were shown in the last refresh
            if (pcol[i].x != OUTSIDE){//Happens only if its the first column
                for(int j=i; pcol[j].x <= NUM_COL ; j++){
                    if(j==i){
                        pcol[i].num_col=NUM_COL;//The first one is going to be the last one
                    }
                    else{
                        pcol[j].num_col--;
                    }
                }
            }
            column_t next_coord= i==0 ? pcol[NUM_COL-1]: pcol[i-1]; //Asures that we are in the arr range
            if(next_coord.x==OUTSIDE || (((next_coord.len)+next_coord.x) >= WIDTH-SPACE) ){ //Case coord of the next column are nearer than SPACE+COL_WIDTH -> col[i] is still OUTSIDE
                pcol[i].x=OUTSIDE;
            }
            else{ //Column reapears in the screen 
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
void set_parameters(int new_height, int new_width){
    if (new_height < 30 || new_width < 30) {
        endwin();
        perror("The screen is too small");
        exit(1);
    }
    HEIGHT=new_height;
    WIDTH=new_width;
    HOLE_HEIGHT = HEIGHT / 3; //Default value
    COL_WIDTH = WIDTH / 15; //Default value
    SPACE = HEIGHT / 2; //SPACE between col
    NUM_COL = WIDTH / COL_WIDTH; //max num of col
}

/*Update screen size fun- NOT WORKING*/

int update_screen_dimensions(void) {
    int new_height;
    int new_width;
    getmaxyx(stdscr, new_height, new_width);

    int old_num_col=NUM_COL;
    int old_col_width=COL_WIDTH;
    int old_space= SPACE;
    set_parameters(new_height, new_width);

    column_t* pnew_column = realloc(column, NUM_COL * sizeof(column_t));
    if (pnew_column == NULL) {
        endwin();
        perror("Error al realocar columnas");
        exit(1);
    }

    // Inicializar nuevas columnas si se agrandó
    int col_aux[NUM_COL]; //Saves the index in the real arr of col is ordered in appearing orden
    for(int i=first_col_index(), k=0;k<NUM_COL;i++,k++){
        if(i==NUM_COL){
            i=0;
        }
        col_aux[k]=i;
    }

    if(old_num_col>=NUM_COL){

        int i;
        for( i=0; pnew_column[col_aux[i]].x != OUTSIDE ;i++){//Organices the col that were shown before
            pnew_column[col_aux[i]].len+=(COL_WIDTH-old_col_width); //Sets the length of the column
            if (pnew_column[col_aux[i]].num_col!=1){
                (pnew_column[col_aux[i]].x)+=(COL_WIDTH-old_col_width)+(SPACE-old_space);  //Sets the coordinate x of each point of the column adding the extra space   
            }
            
        }
        //Oreganization of the new columns
        int aux_x=pnew_column[col_aux[i]].x;
        for(;i<NUM_COL;i++){ //set new col
            aux_x+=SPACE+COL_WIDTH;  
            pnew_column[col_aux[i]].num_col=i;
            if(aux_x >WIDTH){//set the OUTSIDE COL
                pnew_column[col_aux[i]].x=OUTSIDE;
                pnew_column[col_aux[i]].len=0;
                pnew_column[col_aux[i]].num_col=i;
            }
            else{
                pnew_column[col_aux[i]].x=aux_x;  //Sets the coordinate x of each point of the column
                pnew_column[col_aux[i]].y=rand_hole();
                if(aux_x>(WIDTH-COL_WIDTH)){
                    pnew_column[col_aux[i]].len=(WIDTH-COL_WIDTH); //Sets the length of the column
                }
            }
        }
    }
    
    column = pnew_column;// Update global pointer

    return 1;
}


void handle_winch(int sig) {
    endwin();
    refresh();
    clear();
    update_screen_dimensions();
}

int first_col_index(void){
    for( int i =0; i< NUM_COL ;i++){
        if (column[i].num_col==1){
            return i;
        }
    }
    return -1;
}

    