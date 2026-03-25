#ifndef SCREEN_BASE_H
#define SCREEN_BASE_H

#include "utilities.h"
#include "lcd.h"

typedef struct{
    lv_obj_t *obj;
    void (*update_function)(lv_obj_t*, dilemma_status_t current_status, dilemma_status_t prev_status);
} obj_update_dilemma_lcd_status_t;

void init_screen_base(void) ;
void refresh_screen_base(void);
void load_module_base(void);
const char *rgb_matrix_get_effect_name(void);

#endif