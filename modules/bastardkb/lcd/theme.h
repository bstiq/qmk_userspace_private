#include "lvgl.h"

typedef struct {
    const lv_font_t *font;
    uint8_t          border_radius;
    uint16_t         bg_opacity;
    lv_color_t       bg_color;
    lv_color_t       border_color;
    lv_color_t       text_color;
    uint8_t          border_width;
    lv_color_t       bg_grad_color;
    lv_grad_dir_t    bg_grad_dir;
    lv_color_t       shadow_color;
    int16_t          shadow_width;
    int16_t          shadow_ofs_x;
    int16_t          shadow_ofs_y;
    int16_t          shadow_spread;
    lv_color_t       outline_color;
    lv_coord_t       outline_width;
    lv_opa_t         outline_opa;
    lv_coord_t       outline_pad;
    lv_opa_t         text_opa;
} ui_theme_mod_btn;

typedef struct {
    const lv_font_t *font;
    uint8_t          border_radius;
    lv_color_t       border_color;
    lv_color_t       text_color;
    lv_color_t       bg_color;
    uint8_t          border_width;
} ui_theme_layer_name;

typedef struct {
    const lv_font_t *font;
    lv_color_t       text_color;
} ui_theme_secondary_label;

typedef struct {
    uint8_t    border_radius;
    lv_color_t bg_color;
    uint16_t   height;
    uint16_t   bg_opacity;
} ui_theme_bar;

typedef struct {
    uint8_t border_radius;
    uint8_t border_width;
} ui_theme_bar_background;

typedef struct {
    lv_color_t bg_color;
} ui_theme_container;

typedef struct {
    ui_theme_container           container;
    ui_theme_mod_btn         btn_normal;
    ui_theme_mod_btn         btn_pressed;
    ui_theme_layer_name      layer_name;
    ui_theme_secondary_label secondary_labels;
    ui_theme_bar             bar;
    ui_theme_bar_background  bar_background;
    bool                     change_colors_on_layer_change;
    // SEPARATOR LINE -- TODO
} ui_theme;

/*
    This structure is used to store the LCD configuration in eeprom.
    At the moment it only stores the theme id.
    Ideally we would sync all the data (rgb, dpi, etc) across both halves - but this needs to be done in dilemma.c directly.
    Meanwhile, we trigger a manual sync when the keyboard starts up.
*/
// TODO remove raw?
// TODO add brightness configuration
typedef union {
    uint8_t raw;
    struct {
        uint8_t current_theme_id;
    } __attribute__((packed));
} dilemma_status_theme_t;

dilemma_status_theme_t dilemma_lcd_theme;

// TODO once removed ui_styles from lcd.c, move this into ui_elements.c to make it private to ui_elements 
typedef struct {
    lv_style_t mod_btn;
    lv_style_t bar;
    lv_style_t bar_background;
    lv_style_t mod_btn_pressed;
    lv_style_t flex_container;
    lv_style_t line;
    lv_style_t layer_name;
    lv_style_t line_background;
    lv_style_t secondary_labels;
} ui_styles_t;

ui_styles_t ui_styles;

void init_themes(void);
void update_styles_from_theme_btn(lv_style_t *btn, ui_theme_mod_btn theme_btn);
void update_styles_from_theme_layer_name(lv_style_t *style, ui_theme_layer_name theme);
void update_styles_from_theme_secondary_label(lv_style_t *style, ui_theme_secondary_label theme);
void update_styles_from_theme_bar(lv_style_t *style, ui_theme_bar theme);
void update_styles_from_theme_bar_background(lv_style_t *style, ui_theme_bar_background theme);
void update_styles_from_theme_container(lv_style_t *style, ui_theme_container theme);
void read_dilemma_theme_config_from_eeprom(dilemma_status_theme_t *config);
void load_dilemma_theme_config_from_eeprom(void);
void write_dilemma_theme_config_to_eeprom(dilemma_status_theme_t *config);

void     init_styles(void);
void     change_style_colors(HSV hsv);
ui_theme get_current_theme(void);
void     update_styles_from_current_theme(void);
uint8_t  get_current_theme_id(void);
void set_current_theme_id(uint8_t id);