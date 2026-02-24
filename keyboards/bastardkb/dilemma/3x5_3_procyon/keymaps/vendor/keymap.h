painter_font_handle_t  bk_font_layer;
painter_font_handle_t  bk_font_menu;
painter_font_handle_t  bk_font_menu_off;
painter_image_handle_t rect_mods;
static uint8_t         prev_layer;
// static uint8_t        last_mods;

#undef RP_I2C_USE_I2C0
#define RP_I2C_USE_I2C0 FALSE

#undef RP_I2C_USE_I2C1
#define RP_I2C_USE_I2C1 TRUE

const char *bk_layer_str(enum dilemma_keymap_layers layer);
void        bk_display_layer_name(int x, int y, int layer, painter_font_handle_t font);
int         bk_display_layer_info(int x, int y, int layer, painter_font_handle_t font, bool rewrite_all);

int  bk_display_info_base(uint16_t x, uint16_t y, painter_font_handle_t font_on, painter_font_handle_t font_off, bool render_all);
int  bk_display_info_pointer(uint16_t x, uint16_t y, painter_font_handle_t font_on, painter_font_handle_t font_off, bool render_all);
int  bk_display_info_media(uint16_t x, uint16_t y, painter_font_handle_t font_on, painter_font_handle_t font_off, bool render_all);
void bk_drawtext_off(int x, int y, painter_font_handle_t font, const char *str);

static const char *layer_strings[] = {"BASE", "FUNCTION", "NAV", "MED/RGB", "POINTER", "NUM", "SYM"};