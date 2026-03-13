#include QMK_KEYBOARD_H
#include "introspection.h"
#include "lvgl.h"
#include <ctype.h>
#include "theme.h"

void keyboard_post_init_lcd(void);
void style_init_all(void);

void              housekeeping_task_lcd(void);
void              update_mods(bool force);
void              update_mouse_info(bool force);
void              update_rgb_info(bool force);
void              update_layer_name(bool force);
void              update_theme_color(bool force);

void update_dilemma_status(void);

const char *rgb_matrix_get_effect_name(void);

void      update_styles(ui_theme theme);
bool      process_record_lcd(uint16_t keycode, keyrecord_t *record);
ui_theme  get_current_theme(void);

void mouse_info_sync_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data);
void refresh_lcd_info(bool force);
void cycle_theme_and_save_in_eeprom(void);
void init_display(void);
void write_config_to_eeprom(void);