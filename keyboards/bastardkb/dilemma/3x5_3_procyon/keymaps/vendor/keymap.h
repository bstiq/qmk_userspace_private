painter_font_handle_t  bk_font_layer;
painter_font_handle_t  bk_font_menu;
painter_font_handle_t  bk_font_menu_off;
painter_image_handle_t rect_mods;
// static uint8_t         prev_layer;
// static uint8_t        prev_mods;

#undef RP_I2C_USE_I2C0
#define RP_I2C_USE_I2C0 FALSE

#undef RP_I2C_USE_I2C1
#define RP_I2C_USE_I2C1 TRUE