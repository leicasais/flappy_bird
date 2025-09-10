#ifndef RUNNING_FUN_H
#define RUNNING_FUN_H

    //Incluide Typedefs and Macros
    #include "structs.h"

    //column_fun.c
    int rand_hole(screen_dim_t *screen_dim);
    void col_mov(column_t* pcol, screen_dim_t *screen_dim, menu_t *pmenu);
    void col_reset_scroll(void);
  
    //bird_fun.c
    void update_bird_animation(bird_t *bird);
    void bird_flying(bird_t *bird);
    void bird_jump(bird_t* bird);
    void bird_fall(bird_t* bird, screen_dim_t *screen_dim);
    void set_bird_skin(bird_t *bird, app_t *app, int idx);

#endif
