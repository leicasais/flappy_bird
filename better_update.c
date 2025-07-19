/*int update_screen_dimensions(void) {
    int new_height;
    int new_width;
    getmaxyx(stdscr, new_height, new_width);

    if (new_height < 30 || new_width < 30) {
        endwin();
        perror("The screen is too small");
        exit(1);
        return -1;
    }

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
    if(old_num_col>=NUM_COL){
        int first= first_col_index();
        int i;
        for( i=first; pnew_column[i].x != OUTSIDE;i++){//Organices the col that were shown before
            pnew_column[i].len+=(COL_WIDTH-old_col_width); //Sets the length of the column
            if (pnew_column[i].num_col!=1){
                (pnew_column[i].x)+=(COL_WIDTH-old_col_width)+(SPACE-old_space);  //Sets the coordinate x of each point of the column adding the extra space   
            }
            
        }
        //Oreganization of the new columns
        int aux_x=pnew_column[i].x;
        int j;
        for(j =i;j<NUM_COL ;j++){ //set new col
            aux_x+=SPACE+COL_WIDTH;  
            pnew_column[j].num_col=pnew_column[i].num_col+(j-i);
            if(aux_x >WIDTH){//set the OUTSIDE COL
                for(int k=j;k<NUM_COL;j++){ //col outside the screen prepearing to enter
                    pnew_column[k].x=OUTSIDE;
                    pnew_column[k].len=0;
                    pnew_column[k].num_col=j+1;
                }
            }
            else{
                pnew_column[j].x=aux_x;  //Sets the coordinate x of each point of the column
                pnew_column[j].y=rand_hole();
                if(aux_x>(WIDTH-COL_WIDTH)){
                    pnew_column[j].len=(WIDTH-COL_WIDTH); //Sets the length of the column
                }
            }
        }
        
        for(int k=j;k<NUM_COL;j++){ //col outside the screen prepearing to enter
            pnew_column[k].x=OUTSIDE;
            pnew_column[k].len=0;
            pnew_column[k].num_col=j+1;
        }
    }
    
    column = pnew_column;// Update global pointer

    return 1;
}
*/