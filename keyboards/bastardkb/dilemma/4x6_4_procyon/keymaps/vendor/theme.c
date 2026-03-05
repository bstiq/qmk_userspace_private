#include "theme.h"

LV_FONT_DECLARE(montserratbold14);
LV_FONT_DECLARE(montserratbold13);

ui_theme default_theme;
ui_theme skeu_dark_theme;

void init_themes(void) {
    default_theme = (ui_theme){
        .btn_normal =
            {
                .font          = &montserratbold14,
                .border_radius = 6,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(23, 26, 31),
                .border_color  = lv_color_make(50, 55, 67),
                .text_color    = lv_color_white(),
                .border_width  = 2,
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
                .font = &montserratbold13,
            },
        .bar =
            {
                .border_radius = 0,
                .bg_color      = lv_color_make(71, 133, 239),
            },
        .bar_background =
            {
                .border_radius = 3,
                .border_width  = 0,
            },
    };
    skeu_dark_theme = (ui_theme){
        .btn_normal =
            {
                .font          = &montserratbold14,
                .border_radius = 8,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(38, 38, 38),
                .border_color  = lv_color_make(15, 15, 15),
                .text_color    = lv_color_make(46, 46, 46),
                .border_width  = 1,
                .bg_grad_color = lv_color_make(23, 26, 31),
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
        .btn_pressed =
            {
                .font          = &montserratbold14,
                .border_radius = 8,
                .bg_opacity    = LV_OPA_COVER,
                .bg_color      = lv_color_make(29, 29, 29),
                .border_color  = lv_color_make(8, 8, 8),
                .text_color    = lv_color_make(217, 217, 217),
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
                .font          = &montserratbold14,
                .border_radius = 6,
                .bg_color      = lv_color_make(26, 26, 26),
                .border_color  = lv_color_make(12, 12, 12),
                .text_color    = lv_color_make(166, 166, 166),
                .border_width  = 1,
            },
        .secondary_labels =
            {
                .font       = &montserratbold13,
                .text_color = lv_color_make(46, 46, 46),
            },
        .bar =
            {
                .border_radius = 5,
                .bg_color      = lv_color_make(48, 32, 0),
            },
        .bar_background =
            {
                .border_radius = 5,
                .border_width  = 0,
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
    lv_style_set_text_color(style, theme.text_color);
}

void apply_theme_bar(lv_style_t *style, ui_theme_bar theme) {
    lv_style_set_radius(style, theme.border_radius);
    lv_style_set_bg_color(style, theme.bg_color);
}

void apply_theme_bar_background(lv_style_t *style, ui_theme_bar_background theme) {
    lv_style_set_radius(style, theme.border_radius);
    lv_style_set_border_width(style, theme.border_width);
}