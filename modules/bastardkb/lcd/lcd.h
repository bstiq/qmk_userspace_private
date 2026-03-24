#include QMK_KEYBOARD_H
#include "introspection.h"
#include "lvgl.h"
#include <ctype.h>
#include "theme.h"

// todo define bits
typedef struct {
    uint8_t                mods;
    bool                   sniping;
    bool                   scrolling;
    uint8_t                layer;
    uint8_t                current_theme_id;
    uint8_t                rgb_enabled;
    uint8_t                rgb_effect_mode;
    uint16_t               rgb_val;
    uint16_t               dpi;
    uint16_t               s_dpi;
} dilemma_status_t;

typedef struct{
    void (*load_module)(void);
    void (*init_module)(void);
    void (*load_custom_theme_elements)(void);
    void (*update_custom_elements_styles_from_current_theme)(void);
    void (*refresh_module)(void);
} lcd_module_t;

void keyboard_post_init_lcd(void);

void              housekeeping_task_lcd(void);
void              update_theme_color(void);

void update_dilemma_status(void);


bool      process_record_lcd(uint16_t keycode, keyrecord_t *record);

void mouse_info_sync_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data);
void refresh_lcd_info(void);
void cycle_theme_and_save_in_eeprom(void);
void init_display(void);
void write_config_to_eeprom(void);