#include "lvgl.h"
#include <ctype.h>
#include "theme.h"
#include QMK_KEYBOARD_H

// QP stuff
#define SPI_SCK_PIN GP22  // as per vik connector
#define SPI_MOSI_PIN GP23 // as per vik connector
#define SPI_MISO_PIN GP18 // Unused
#define LCD_RST_PIN GP16  // Unused, TODO connect to Vcc for now, test with SCL later
#define LCD_DC_PIN GP12   // vik_gp1
#define LCD_CS_PIN GP11   // vik_gp2
#define LCD_BLK_PIN GP4   // not used
#ifdef BACKLIGHT_LEVELS
#    undef BACKLIGHT_LEVELS
#endif
#define BACKLIGHT_LEVELS 16
#define BACKLIGHT_PIN GP20 // backlight pin for lcd
#define BACKLIGHT_PWM_DRIVER PWMD2
#define BACKLIGHT_PWM_CHANNEL RP2040_PWM_CHANNEL_A

#define SPI_DRIVER SPID0
#define LCD_SPI_DIVISOR 2
#define LCD_WAIT_TIME 150
#define LCD_ROTATION QP_ROTATION_0
#define LCD_OFFSET_X 0
#define LCD_OFFSET_Y 15
#define LCD_WIDTH 240  // Set according to your display specs
#define LCD_HEIGHT 280 // Set according to your display specs
#define SPI_MODE 3     // Set according to your display specs
#define ST7789         // Set according to your display specs GC_9A01 or ST7789
#ifdef QUANTUM_PAINTER_SUPPORTS_NATIVE_COLORS
#    undef QUANTUM_PAINTER_SUPPORTS_NATIVE_COLORS
#endif
#define QUANTUM_PAINTER_SUPPORTS_NATIVE_COLORS TRUE
// Timeout configuration, default 30000 (30 sek). 0 = No timeout. Beware of image retention.
#ifdef QUANTUM_PAINTER_DISPLAY_TIMEOUT
#    undef QUANTUM_PAINTER_DISPLAY_TIMEOUT
#endif
#define QUANTUM_PAINTER_DISPLAY_TIMEOUT 0
// #define QUANTUM_PAINTER_PIXDATA_BUFFER_SIZE 4096
#ifdef QUANTUM_PAINTER_TASK_THROTTLE
#    undef QUANTUM_PAINTER_TASK_THROTTLE
#endif
#define QUANTUM_PAINTER_TASK_THROTTLE 16 // 60 FPS ish
#define RGB_MATRIX_MODE_NAME_ENABLE
#ifdef QP_LVGL_TASK_PERIOD
#    undef QP_LVGL_TASK_PERIOD
#endif
#define QP_LVGL_TASK_PERIOD 33 // throttle lvgl for keyboard responsiveness

// #define QUANTUM_PAINTER_DISPLAY_TIMEOUT 5000

// Custom display stuff
#define BK_PALETTE LV_PALETTE_CYAN

#undef RP_I2C_USE_I2C0
#define RP_I2C_USE_I2C0 FALSE

#undef RP_I2C_USE_I2C1
#define RP_I2C_USE_I2C1 TRUE
// end QP stuff

typedef struct mod_button_pair_t mod_button_pair_t;

void keyboard_post_init_lcd(void);
void style_init_all(void);

void              ui_init_layer_name(lv_obj_t *label);
void              housekeeping_task_lcd(void);
void              update_mods(void);
void              update_mouse_info(void);
void              update_rgb_info(void);
void              update_rgb_info(void);
void              update_layer_name(void);
void              ui_init_button_mod_indicator(lv_obj_t *label);
mod_button_pair_t ui_create_mod_button(lv_obj_t *cont, const char *text, bool force_new_track, uint8_t mod_mask);

void event_screen_base_update_mods(lv_event_t *e);
void event_screen_pointer_sniping_toggle(lv_event_t *e);
void event_screen_pointer_scroll_toggle(lv_event_t *e);

void update_theme_color(void);
void update_dilemma_status(void);

const char *rgb_matrix_get_effect_name(void);

lv_obj_t *ui_create_secondary_text(lv_obj_t *cont, const char *text, bool new_track, uint8_t flex);
lv_obj_t *ui_create_progress_bar(lv_obj_t *cont, uint8_t flex);
lv_obj_t *ui_create_number_label(lv_obj_t *cont, uint8_t flex);
lv_obj_t *ui_create_line_separator(lv_obj_t *cont, uint8_t flex, uint8_t height);
void      update_styles(ui_theme theme);