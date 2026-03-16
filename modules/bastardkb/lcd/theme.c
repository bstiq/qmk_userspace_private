#include QMK_KEYBOARD_H
#include "theme.h"
#include "ui_elements.h"

ui_theme  default_theme;
ui_theme  skeu_dark_theme;
ui_theme  terminal_theme;
ui_theme  ivory_theme;
ui_theme  steel_theme;
ui_theme  oxide_theme;
ui_theme *themes[] = {&default_theme, &skeu_dark_theme, &terminal_theme, &ivory_theme, &steel_theme, &oxide_theme};

LV_FONT_DECLARE(montserratbold14);
LV_FONT_DECLARE(montserratbold13);
LV_FONT_DECLARE(dmsans13);
LV_FONT_DECLARE(dmsans14);
LV_FONT_DECLARE(dmsans14bold);
LV_FONT_DECLARE(jetbrainsmono13);
LV_FONT_DECLARE(jetbrainsmono14bold);

void init_themes(void) {
    default_theme = (ui_theme){

        .container =
            {
                .bg_color = lv_color_make(0, 0, 0),
            },
        .change_colors_on_layer_change = false,
        .btn_normal =
            {
                .font          = &montserratbold14,
                .border_radius = 6,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(23, 26, 31),
                .border_color  = lv_color_make(50, 55, 67),
                .text_color    = lv_color_white(),
                .border_width  = 2,
                .text_opa      = LV_OPA_COVER,
            },
        .btn_pressed =
            {
                .font          = &montserratbold14,
                .border_radius = 6,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(71, 133, 239),
                .border_color  = lv_color_white(),
                .text_color    = lv_color_white(),
                .border_width  = 2,
                .text_opa      = LV_OPA_COVER,
            },
        .layer_name =
            {
                .font          = &montserratbold14,
                .border_radius = 6,
                .bg_color      = lv_color_black(),
                .border_color  = lv_color_make(50, 55, 67),
                .text_color    = lv_color_white(),
                .border_width  = 2,
            },
        .secondary_labels =
            {
                .font       = &montserratbold13,
                .text_color = lv_color_make(150, 150, 150),
            },
        .bar =
            {
                .border_radius = 0,
                .bg_color      = lv_color_make(71, 133, 239),
                .height        = 6,
                .bg_opacity    = LV_OPA_COVER,
            },
        .bar_background =
            {
                .border_radius = 3,
                .border_width  = 0,
            },
    };
    skeu_dark_theme = (ui_theme){

        .container =
            {
                .bg_color = lv_color_make(0, 0, 0),
            },
        .btn_normal =
            {
                .font          = &dmsans14bold,
                .border_radius = 8,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(38, 38, 38),
                .border_color  = lv_color_make(15, 15, 15),
                .text_color    = lv_color_make(150, 150, 150),
                .border_width  = 1,
                .bg_grad_color = lv_color_make(23, 26, 31),
                .bg_grad_dir   = LV_GRAD_DIR_VER,
                .text_opa      = LV_OPA_COVER,
                .shadow_color  = lv_color_black(),
                .shadow_width  = 0,
                .shadow_ofs_x  = 0,
                .shadow_ofs_y  = 0,
                .shadow_spread = 0,
                .outline_color = lv_color_black(),
                .outline_width = 0,
                .outline_opa   = LV_OPA_TRANSP,
                .outline_pad   = 0,
            },
        .btn_pressed =
            {
                .font          = &dmsans14bold,
                .border_radius = 8,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(29, 29, 29),
                .border_color  = lv_color_make(8, 8, 8),
                .text_color    = lv_color_make(232, 152, 10),
                .border_width  = 1,
                .bg_grad_color = lv_color_make(30, 22, 0),
                .bg_grad_dir   = LV_GRAD_DIR_VER,
                .text_opa      = LV_OPA_COVER,
                .shadow_color  = lv_color_make(200, 130, 0),
                .shadow_width  = 20,
                .shadow_ofs_x  = 0,
                .shadow_ofs_y  = 0,
                .shadow_spread = 0,
                .outline_color = lv_color_make(200, 130, 0),
                .outline_width = 1,
                .outline_opa   = LV_OPA_20,
                .outline_pad   = 4,
            },
        .layer_name =
            {
                .font          = &dmsans14bold,
                .border_radius = 6,
                .bg_color      = lv_color_make(26, 26, 26),
                .border_color  = lv_color_make(12, 12, 12),
                .text_color    = lv_color_make(166, 166, 166),
                .border_width  = 1,
            },
        .secondary_labels =
            {
                .font       = &dmsans13,
                .text_color = lv_color_make(150, 150, 150),
            },
        .bar =
            {
                .border_radius = 3,
                .bg_color      = lv_color_make(232, 152, 10),
                .height        = 4,
                .bg_opacity    = LV_OPA_COVER,
            },
        .bar_background =
            {
                .border_radius = 3,
                .border_width  = 0,
            },
    };
    terminal_theme = (ui_theme){
        // Green phosphor CRT — inactive: near-black bg, dim green border/text

        .container =
            {
                .bg_color = lv_color_make(0, 0, 0),
            },
        .btn_normal =
            {
                .font          = &jetbrainsmono14bold,
                .border_radius = 0,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(5, 14, 5),
                .border_color  = lv_color_make(26, 118, 26),
                .text_color    = lv_color_make(42, 166, 42),
                .border_width  = 1,
                .bg_grad_color = lv_color_make(5, 14, 5),
                .bg_grad_dir   = LV_GRAD_DIR_NONE,
                .text_opa      = LV_OPA_COVER,
                .shadow_color  = lv_color_black(),
                .shadow_width  = 0,
                .shadow_ofs_x  = 0,
                .shadow_ofs_y  = 0,
                .shadow_spread = 0,
                .outline_color = lv_color_black(),
                .outline_width = 0,
                .outline_opa   = LV_OPA_TRANSP,
                .outline_pad   = 0,
            },
        // Active/pressed: solid phosphor green fill, dark text — like .term-mode-btn.on
        .btn_pressed =
            {
                .font          = &jetbrainsmono14bold,
                .border_radius = 0,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(0, 255, 65),
                .border_color  = lv_color_make(0, 255, 65),
                .text_color    = lv_color_make(5, 14, 5),
                .border_width  = 1,
                .bg_grad_color = lv_color_make(0, 255, 65),
                .bg_grad_dir   = LV_GRAD_DIR_NONE,
                .text_opa      = LV_OPA_COVER,
                .shadow_color  = lv_color_make(0, 255, 65),
                .shadow_width  = 10,
                .shadow_ofs_x  = 0,
                .shadow_ofs_y  = 0,
                .shadow_spread = 0,
                .outline_color = lv_color_make(0, 255, 65),
                .outline_width = 1,
                .outline_opa   = LV_OPA_20,
                .outline_pad   = 3,
            },
        // Layer name: bright phosphor green text on near-black bg
        .layer_name =
            {
                .font          = &jetbrainsmono14bold,
                .border_radius = 0,
                .bg_color      = lv_color_make(5, 14, 5),
                .border_color  = lv_color_make(26, 118, 26),
                .text_color    = lv_color_make(0, 255, 65),
                .border_width  = 1,
            },
        // Secondary labels: dim green, like .term-key / .term-layer-sub
        .secondary_labels =
            {
                .font       = &jetbrainsmono13,
                .text_color = lv_color_make(42, 166, 42),
            },
        // Bar fill: bright phosphor green, sharp corners
        .bar =
            {
                .border_radius = 0,
                .bg_color      = lv_color_make(0, 255, 65),
                .height        = 4,
                .bg_opacity    = LV_OPA_COVER,
            },
        // Bar background: dark green trough, no border radius
        .bar_background =
            {
                .border_radius = 0,
                .border_width  = 1,
            },
    };
    ivory_theme = (ui_theme){
        .change_colors_on_layer_change = false,
        .container =
            {
                .bg_color = lv_color_make(255, 255, 240),
            },
        .btn_normal =
            {
                .font          = &montserratbold14,
                .border_radius = 8,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(248, 245, 240),
                .border_color  = lv_color_make(192, 188, 181),
                .text_color    = lv_color_make(80, 75, 68),
                .border_width  = 1,
                .text_opa      = LV_OPA_COVER,
            },
        .btn_pressed =
            {
                .font          = &montserratbold14,
                .border_radius = 8,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(204, 228, 206),
                .border_color  = lv_color_make(158, 203, 164),
                .text_color    = lv_color_make(31, 92, 41),
                .border_width  = 1,
                .text_opa      = LV_OPA_COVER,
            },
        .layer_name =
            {
                .font          = &montserratbold14,
                .border_radius = 8,
                .bg_color      = lv_color_make(238, 234, 228),
                .border_color  = lv_color_make(192, 188, 181),
                .text_color    = lv_color_make(60, 56, 50),
                .border_width  = 1,
            },
        .secondary_labels =
            {
                .font       = &montserratbold13,
                .text_color = lv_color_make(140, 136, 128),
            },
        .bar =
            {
                .border_radius = 5,
                .bg_color      = lv_color_make(41, 122, 55),
                .height        = 9,
                .bg_opacity    = LV_OPA_COVER,
            },
        .bar_background =
            {
                .border_radius = 5,
                // .bg_color      = lv_color_make(216, 212, 206),
                .border_width = 0,
            },
    };
    steel_theme = (ui_theme){

        .container =
            {
                .bg_color = lv_color_make(0, 0, 0),
            },
        .change_colors_on_layer_change = false,
        .btn_normal =
            {
                .font          = &montserratbold14,
                .border_radius = 8,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(32, 40, 54),
                .border_color  = lv_color_make(12, 16, 24),
                .text_color    = lv_color_make(255, 255, 255),
                .border_width  = 2,
                .text_opa      = LV_OPA_COVER,
            },
        .btn_pressed =
            {
                .font          = &montserratbold14,
                .border_radius = 8,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(12, 24, 48),
                .border_color  = lv_color_make(26, 46, 80),
                .text_color    = lv_color_make(91, 156, 246),
                .border_width  = 2,
                .text_opa      = LV_OPA_COVER,
            },
        .layer_name =
            {
                .font          = &montserratbold14,
                .border_radius = 8,
                .bg_color      = lv_color_make(20, 26, 34),
                .border_color  = lv_color_make(10, 12, 22),
                .text_color    = lv_color_make(255, 255, 255),
                .border_width  = 2,
            },
        .secondary_labels =
            {
                .font       = &montserratbold13,
                .text_color = lv_color_make(80, 96, 128),
            },
        .bar =
            {
                .border_radius = 5,
                .bg_color      = lv_color_make(91, 156, 246),
                .height        = 9,
                .bg_opacity    = LV_OPA_COVER,
            },
        .bar_background =
            {
                .border_radius = 5,
                // .bg_color      = lv_color_make(10,  14,  24),
                .border_width = 0,
            },
    };
    oxide_theme = (ui_theme){

        .container =
            {
                .bg_color = lv_color_make(0, 0, 0),
            },
        .change_colors_on_layer_change = false,
        .btn_normal =
            {
                .font          = &montserratbold14,
                .border_radius = 8,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(46, 32, 30),
                .border_color  = lv_color_make(16, 12, 10),
                .text_color    = lv_color_make(255, 255, 255),
                .border_width  = 2,
                .text_opa      = LV_OPA_COVER,
            },
        .btn_pressed =
            {
                .font          = &montserratbold14,
                .border_radius = 8,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(32, 12, 8),
                .border_color  = lv_color_make(58, 24, 16),
                .text_color    = lv_color_make(224, 90, 58),
                .border_width  = 2,
                .text_opa      = LV_OPA_COVER,
            },
        .layer_name =
            {
                .font          = &montserratbold14,
                .border_radius = 8,
                .bg_color      = lv_color_make(30, 20, 18),
                .border_color  = lv_color_make(12, 10, 8),
                .text_color    = lv_color_make(255, 255, 255),
                .border_width  = 2,
            },
        .secondary_labels =
            {
                .font       = &montserratbold13,
                .text_color = lv_color_make(110, 80, 72),
            },
        .bar =
            {
                .border_radius = 5,
                .bg_color      = lv_color_make(224, 90, 58),
                .height        = 9,
                .bg_opacity    = LV_OPA_COVER,
            },
        .bar_background =
            {
                .border_radius = 5,
                // .bg_color      = lv_color_make(14,  10,  8),
                .border_width = 0,
            },
    };
}

void update_styles_from_theme_btn(lv_style_t *style, ui_theme_mod_btn theme) {
    lv_style_init(style);
    lv_style_set_text_font(style, theme.font);
    lv_style_set_radius(style, theme.border_radius);
    lv_style_set_bg_opa(style, theme.bg_opacity);
    lv_style_set_bg_color(style, theme.bg_color);
    lv_style_set_border_color(style, theme.border_color);
    lv_style_set_border_width(style, theme.border_width);
    lv_style_set_text_color(style, theme.text_color);

    lv_style_set_bg_grad_color(style, theme.bg_grad_color);
    lv_style_set_bg_grad_dir(style, theme.bg_grad_dir);
    lv_style_set_text_opa(style, theme.text_opa);
    lv_style_set_shadow_color(style, theme.shadow_color);
    lv_style_set_shadow_width(style, theme.shadow_width);
    lv_style_set_shadow_ofs_x(style, theme.shadow_ofs_x);
    lv_style_set_shadow_ofs_y(style, theme.shadow_ofs_y);
    lv_style_set_shadow_spread(style, theme.shadow_spread);
    lv_style_set_outline_color(style, theme.outline_color);
    lv_style_set_outline_width(style, theme.outline_width);
    lv_style_set_outline_opa(style, theme.outline_opa);
    lv_style_set_outline_pad(style, theme.outline_pad);
    lv_obj_report_style_change(style);
}

void update_styles_from_theme_layer_name(lv_style_t *style, ui_theme_layer_name theme) {
    lv_style_init(style);
    lv_style_set_text_font(style, theme.font);
    lv_style_set_radius(style, theme.border_radius);
    lv_style_set_bg_color(style, theme.bg_color);
    lv_style_set_border_color(style, theme.border_color);
    lv_style_set_border_width(style, theme.border_width);
    lv_style_set_text_color(style, theme.text_color);
    lv_obj_report_style_change(style);
}

void update_styles_from_theme_secondary_label(lv_style_t *style, ui_theme_secondary_label theme) {
    lv_style_init(style);
    lv_style_set_text_font(style, theme.font);
    lv_style_set_text_color(style, theme.text_color);
    lv_obj_report_style_change(style);
}

void update_styles_from_theme_bar(lv_style_t *style, ui_theme_bar theme) {
    lv_style_init(style);
    lv_style_set_radius(style, theme.border_radius);
    lv_style_set_bg_color(style, theme.bg_color);
    lv_style_set_bg_opa(style, theme.bg_opacity);
    lv_style_set_height(style, theme.height);
    lv_obj_report_style_change(style);
}

void update_styles_from_theme_bar_background(lv_style_t *style, ui_theme_bar_background theme) {
    lv_style_set_radius(style, theme.border_radius);
    lv_style_set_border_width(style, theme.border_width);
    lv_obj_report_style_change(style);
}

void update_styles_from_theme_container(lv_style_t *style, ui_theme_container theme) {
    lv_style_set_bg_color(style, theme.bg_color);
}

void read_dilemma_theme_config_from_eeprom(dilemma_status_theme_t *config) {
    config->raw = eeconfig_read_user() & 0xff;
}

void write_dilemma_theme_config_to_eeprom(dilemma_status_theme_t *config) {
    eeconfig_update_user(config->raw);
}

void load_dilemma_theme_config_from_eeprom(void) {
    read_dilemma_theme_config_from_eeprom(&dilemma_lcd_theme);
}

void init_styles(void) {
    ui_styles = (ui_styles_t){0};
    // mod button
    lv_style_init(&ui_styles.mod_btn);

    // pressed mod indicator
    lv_style_init(&ui_styles.mod_btn_pressed);

    // layer name label
    lv_style_init(&ui_styles.layer_name);

    // separator line
    lv_style_set_radius(&ui_styles.line, 0);
    lv_style_set_bg_color(&ui_styles.line, lv_color_make(50, 55, 67));
    lv_style_set_radius(&ui_styles.line_background, 3);
    lv_style_set_border_width(&ui_styles.line_background, 0);
    lv_style_set_bg_color(&ui_styles.line_background, lv_color_make(50, 55, 67));

    // secondary labels
    lv_style_init(&ui_styles.secondary_labels);

    // bars
    // TODO move this out? this is an apply theme...
    update_styles_from_theme_bar(&(ui_styles.bar), get_current_theme().bar);
    update_styles_from_theme_bar_background(&(ui_styles.bar_background), get_current_theme().bar_background);

    // flex container
    lv_style_set_bg_color(&ui_styles.flex_container, lv_color_black());
    lv_style_set_pad_row(&ui_styles.flex_container, 10);
    lv_style_set_border_width(&ui_styles.flex_container, 0);
    // lv_style_set_pad_all(&ui_styles.flex_container, 0);

    update_styles_from_current_theme();
}

void update_styles_from_current_theme(void) {
    ui_theme theme = get_current_theme();
    update_styles_from_theme_btn(&(ui_styles.mod_btn), theme.btn_normal);
    update_styles_from_theme_btn(&(ui_styles.mod_btn_pressed), theme.btn_pressed);
    update_styles_from_theme_layer_name(&(ui_styles.layer_name), theme.layer_name);
    update_styles_from_theme_secondary_label(&(ui_styles.secondary_labels), theme.secondary_labels);
    update_styles_from_theme_bar(&(ui_styles.bar), theme.bar);
    update_styles_from_theme_bar_background(&(ui_styles.bar_background), theme.bar_background);
    update_styles_from_theme_container(&(ui_styles.flex_container), theme.container);
}

void change_style_colors(HSV hsv) {
    lv_style_set_bg_color(&ui_styles.mod_btn_pressed, lv_color_hsv_to_rgb(hsv.h, hsv.s, hsv.v));
    lv_obj_report_style_change(&ui_styles.mod_btn_pressed);
    lv_style_set_bg_color(&ui_styles.bar, lv_color_hsv_to_rgb(hsv.h, hsv.s, hsv.v));
    lv_obj_report_style_change(&ui_styles.bar);
}

ui_theme get_current_theme(void) {
    // TODO reinstate this safety check
    // uint8_t theme_id = (config.current_theme_id) % (sizeof(themes) / sizeof(ui_theme *));
    return *themes[get_current_theme_id()];
}

uint8_t get_current_theme_id(void) {
    return dilemma_lcd_theme.current_theme_id;
}

void set_current_theme_id(uint8_t id) {
    dilemma_lcd_theme.current_theme_id = id;
}

void cycle_theme_and_save_in_eeprom(void) {
    uint8_t new_id = (dilemma_lcd_theme.current_theme_id + 1) % (sizeof(themes) / sizeof(ui_theme *));
    set_current_theme_id(new_id);
    write_dilemma_theme_config_to_eeprom(&dilemma_lcd_theme);
}