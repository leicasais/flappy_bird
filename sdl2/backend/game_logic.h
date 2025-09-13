#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

    //Incluide Typedefs and Macros
    #include "structs.h"

    //game_logic.c
    char collision(column_t* pcol, bird_t* pbird, screen_dim_t *screen_dim);
    void colition_update(menu_t* pmenu, camera_t* camera);
    void camera_update(camera_t *cam, int *offx, int *offy);
    void camera_start_shake(camera_t *cam, float duration_ms, float amp_px, float freq_hz);
    void points(column_t* pcol, bird_t* pbird, menu_t* menu, screen_dim_t *screen_dim);
    void history_log(menu_t* pmenu);
    void score_init(menu_t *pmenu);
    int score_update(menu_t *pmenu, int new_score);
    void score_save(menu_t *pmenu);
    void game_reset(column_t* pcol, bird_t *bird, menu_t *menu, screen_dim_t *screen_dim);
    
#endif