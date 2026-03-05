#include "theme.h"

LV_FONT_DECLARE(montserratbold14);
LV_FONT_DECLARE(montserratbold13);

ui_theme default_theme = {
    {
        montserratbold14,
        6,
        LV_OPA_COVER,
        lv_color_make(23, 26, 31),
        lv_color_make(50, 55, 67),
        lv_color_white(),
        2,
    },
    {
        montserratbold14,
        6,
        LV_OPA_COVER,
        lv_color_make(71, 133, 239),
        lv_color_white(),
        lv_color_white(),
        .border_width = 2,
    },
    {
        &montserratbold14,
        6,
        lv_color_black(),
        lv_color_make(50, 55, 67),
        lv_color_white(),
        2,
    },
    {
        &montserratbold13,
    },
    {
        0,
        lv_color_make(71, 133, 239),
    },
    {
        3,
        0,
    },
};