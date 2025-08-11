#include "backend.h"
#include "frontend.h"
//Variables Globales
int GAME_WIDTH;              //Stores the WIDTH of the screen
int GAME_HEIGHT;             //Stores the HEIGHT of the screen
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
    timeout(MS_BTW_FRAMES);           // `getch()` waits in ms.Calc FPS: (t[ms]*10^-3)^-1. 50 FPS
    srand(time(NULL));     //plant the seed for rand()

    //signal(SIGWINCH, handle_winch);
    
    int frame=0;    //Counts the frames since the last column update
    int reboot_time=0;
    getmaxyx(stdscr, GAME_HEIGHT, GAME_WIDTH);  // first read of the scren dimations (dinamic)  
    set_parameters();
    column = malloc(sizeof(column_t) * NUM_COL);
    bird_t bird; //init bird
    menu_t menu ={.state=MAIN_MENU};

    /*###############################################
                        MAIN PROGRAM
    ###############################################*/
    
    while (1)
    {
        frame++;
        int ch = getch();
        int selection=0;
        //MENU
        switch (menu.state){
            case RESTART:
                init(column,&bird, &menu);
                menu.state=RUNING;
                frame=0;
                reboot_time=0;
                break;
            case MAIN_MENU:
                selection=0;
                while(menu.state == MAIN_MENU){
                    main_menu(ch, &menu, &selection);
                    display_main_menu(selection);
                    ch = getch();    
                }
                break;
            case PAUSE:
                selection=0;
                while(menu.state == PAUSE){
                    pause_menu(ch, &menu, &selection);
                    display_pause_menu(selection);
                    ch = getch();    
                }
                break;
            case GAME_OVER:
                selection=0;
                while(menu.state == GAME_OVER){
                    game_over_menu(ch, &menu, &selection);
                    display_game_over_menu(selection);
                    ch = getch();
                }
                break;
            case RUNING:
                erase();
                if (ch == 'q' || ch == 'Q') {
                    menu.state=PAUSE; 
                }
                //Update game logic
                if(frame>=VEL_COL){
                    col_mov(column);
                    frame=0;
                    if(reboot_time==0){
                        points(column, &bird, &menu);   //Adds a point if the bird has passed a column only if in game mode
                    }
                }
                bird_mov(&bird, ch);
                //Divide cases in resurrection mode, die, playing normally
                if(collision(column, &bird) && !reboot_time){
                    colition_update(&menu);
                    reboot_time=1;
                }
                else if(reboot_time > 0 && reboot_time < (MS_BTW_FRAMES * VEL_COL*2)){
                    reboot_time++;
                    // show resurrection animation
                    display_resurecting_bird(&bird, ch); // animación con parpadeo

                }
                else{
                    reboot_time = 0;                //Ends the resurection mode
                    draw_bird(&bird, ch);        //Displays the normal bird mode
                }
                //Update display
                display_upper_line(menu);  
                draw_col(column);
                refresh();
                break;
                
            default:
                break;
        }
        if(menu.state==EXIT){
            break;
        }         
    }
    free(column);
    endwin(); // exits ncurses mode, returns control to the terminal
    return 0;
}