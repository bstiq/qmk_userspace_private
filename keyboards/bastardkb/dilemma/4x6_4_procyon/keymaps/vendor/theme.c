#include "theme.h"

LV_FONT_DECLARE(montserratbold14);
LV_FONT_DECLARE(montserratbold13);

ui_theme default_theme;

void init_themes(void) {
    default_theme = (ui_theme){
        .btn_normal = {
            .font = &montserratbold14,
            .border_radius = 6,
            .bg_opacity = LV_OPA_COVER,
            .bg_color = lv_color_make(23, 26, 31),
            .border_color = lv_color_make(50, 55, 67),
            .text_color = lv_color_white(),
            .border_width = 2,
        },
        .btn_pressed = {
            .font = &montserratbold14,
            .border_radius = 6,
            .bg_opacity = LV_OPA_COVER,
            .bg_color = lv_color_make(71, 133, 239),
            .border_color = lv_color_white(),
            .text_color = lv_color_white(),
            .border_width = 2,
        },
        .layer_name = {
            .font = &montserratbold14,
            .border_radius = 6,
            .bg_color = lv_color_black(),
            .border_color = lv_color_make(50, 55, 67),
            .text_color = lv_color_white(),
            .border_width = 2,
        },
        .secondary_labels = {
            .font = &montserratbold13,
        },
        .bar = {
            .border_radius = 0,
            .bg_color = lv_color_make(71, 133, 239),
        },
        .bar_background = {
            .border_radius = 3,
            .border_width = 0,
        },
    };
}

void apply_theme_btn(lv_style_t *style, ui_theme_mod_btn theme) {
    lv_style_set_text_font(style, theme.font);
    lv_style_set_radius(style, theme.border_radius);
    lv_style_set_bg_opa(style, theme.bg_opacity);
    lv_style_set_bg_color(style, theme.bg_color);
    lv_style_set_border_color(style, theme.border_color);
    lv_style_set_border_width(style, theme.border_width);
    lv_style_set_text_color(style, theme.text_color);
}

void apply_theme_layer_name(lv_style_t *style, ui_theme_layer_name theme) {
    lv_style_set_text_font(style, theme.font);
    lv_style_set_radius(style, theme.border_radius);
    lv_style_set_bg_color(style, theme.bg_color);
    lv_style_set_border_color(style, theme.border_color);
    lv_style_set_border_width(style, theme.border_width);
    lv_style_set_text_color(style, theme.text_color);
}

void apply_theme_secondary_label(lv_style_t *style, ui_theme_secondary_label theme) {
    lv_style_set_text_font(style, theme.font);
}

void apply_theme_bar(lv_style_t *style, ui_theme_bar theme) {
    lv_style_set_radius(style, theme.border_radius);
    lv_style_set_bg_color(style, theme.bg_color);
}

void apply_theme_bar_background(lv_style_t *style, ui_theme_bar_background theme) {
    lv_style_set_radius(style, theme.border_radius);
    lv_style_set_border_width(style, theme.border_width);
}