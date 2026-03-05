#include "theme.h"

LV_FONT_DECLARE(montserratbold14);
LV_FONT_DECLARE(montserratbold13);

static ui_theme default_theme = {
    .btn_normal = {
        .font = montserratbold14,
        .border_radius = 6,
        .bg_opacity = LV_OPA_COVER,
        .bg_color = lv_color_make(23, 26, 31),
        .border_color = lv_color_make(50, 55, 67),
        .text_color = lv_color_white(),
        .border_width = 2,
    },
    .btn_pressed = {
        .font = montserratbold14,
        .border_radius = 6,
        .bg_opacity = LV_OPA_COVER,
        .bg_color = lv_color_make(71, 133, 239),
        .border_color = lv_color_white(),
        .text_color = lv_color_white(),
        .border_width = 2,
    },
    .layer_name = {
        .font = montserratbold14,
        .border_radius = 6,
        .bg_color = lv_color_black(),
        .border_color = lv_color_make(50, 55, 67),
        .text_color = lv_color_white(),
        .border_width = 2,
    },
    .secondary_labels = {
        .font = montserratbold13,
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