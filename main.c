#include "backend.h"
#include "frontend.h"
//Variables Globales
int WIDTH;              //Stores the WIDTH of the screen
int HEIGHT;             //Stores the HEIGHT of the screen
int HOLE_HEIGHT;        //Stores the Height of the holes
int NUM_COL;            //Stores the number of columns including the ones OUTSIDE the screen
int COL_WIDTH;          //Stores the WIDTH of the columns
int SPACE;              //Stores the space between the columns
float GRAVITY;   
float JUMP_VEL;   

column_t* column;       //stores all directions of the columns, they are not in order


int main(void)
{
    initscr();  
    cbreak();   //Read keys not waiting for Enter 
    noecho();   
    keypad(stdscr, TRUE);
    curs_set(FALSE);        // Oculta el cursor
    nodelay(stdscr, TRUE);        
    timeout(16);           // `getch()` waits in ms.Calc FPS: (t[ms]*10^-3)^-1. 50 FPS
    srand(time(NULL));     //plant the seed for rand()

    //signal(SIGWINCH, handle_winch);
    getmaxyx(stdscr, HEIGHT, WIDTH);  // first read of the scren dimations (dinamic)  
    set_parameters();

    //var
    column = malloc(sizeof(column_t) * NUM_COL);
    bird_t bird; //init bird
    menu_t menu;
    int frame=0;    //Counts the frames since the last column update

    /*###############################################
    
                        MAIN PROGRAM

    ###############################################*/
    init(column,&bird, &menu);
    
    while (1)
    {
        frame++;
        int ch = getch();
        //MENU
        if(menu.state==MAIN_MENU){
            int selection=0;
            while(1){
                main_menu(ch, &menu, &selection);
                display_main_menu(selection);
                if (menu.state!=MAIN_MENU){
                    break;
                }
                ch = getch();    
            }
        }
        else if(menu.state==PAUSE){//Pasa lo mismo que para main menu por que todavia no setie el modo menu de pausa, despues hay que cambiarlo
            int selection=0;
            while(1){
                main_menu(ch, &menu, &selection);
                display_main_menu(selection);
                if (menu.state!=MAIN_MENU){
                    break;
                }
                ch = getch();    
            }
        }
        else if(menu.state==RUNING){
            //Update game logic
            if(frame==4){
                col_mov(column);
                frame=0;
            }
            
            if (ch == ' ') {
                bird_jump(&bird);
            }
            else if (ch == 'q') {
                menu.state=PAUSE;  
            }
            bird_mov(&bird);


            //Update display
            erase(); 
            display_col(column);
            display_bird(&bird);
            refresh();
        }
        else if(menu.state== EXIT){
            break;
        } 
        
        
    }
    
    free(column);
    endwin(); // exits ncurses mode, returns control to the terminal
    return 0;
}