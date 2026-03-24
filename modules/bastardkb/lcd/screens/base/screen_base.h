#ifndef SCREEN_BASE_H
#define SCREEN_BASE_H

typedef struct{
    lv_obj_t *obj;
    void (*update_function)(lv_obj_t*);
} obj_update_t;

typedef struct{
    obj_update_t *array;
    size_t used;
    size_t size;
    int amount_elements;
}obj_event_array_t;

lv_obj_t *ui_screen_base;

void init_screen_base(void) ;
void refresh_screen_base(void);

void init_obj_event_array(obj_event_array_t *a);
void add_obj_event_array(obj_event_array_t *a, obj_update_t element);

void update_layer_name(lv_obj_t *obj);
void update_rgb_value(lv_obj_t* obj);
void update_rgb_bar(lv_obj_t* obj);
void update_mod_shift(lv_obj_t *obj);
void update_mod_ctrl(lv_obj_t *obj);
void update_mod_alt(lv_obj_t *obj);
void update_mod_gui(lv_obj_t *obj);
void update_mod_snipe(lv_obj_t *obj);
void update_mod_scroll(lv_obj_t *obj);
void update_mod_snipe_dpi_bar(lv_obj_t *obj);
void update_mod_snipe_dpi_number(lv_obj_t *obj);
void update_mod_dpi_bar(lv_obj_t *obj);
void update_mod_dpi_number(lv_obj_t *obj);
void update_mod_xx(lv_obj_t *obj, uint8_t mod_mask);
void update_rgb_effect(lv_obj_t *obj);
const char *rgb_matrix_get_effect_name(void);

#endif