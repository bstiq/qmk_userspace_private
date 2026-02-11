painter_font_handle_t bk_font_layer;
painter_font_handle_t bk_font_menu;
painter_font_handle_t bk_font_menu_off;
static uint8_t        prev_layer;
// static uint8_t        last_mods;

#undef RP_I2C_USE_I2C0
#define RP_I2C_USE_I2C0 FALSE

#undef RP_I2C_USE_I2C1
#define RP_I2C_USE_I2C1 TRUE

const char *bk_layer_str(enum dilemma_keymap_layers layer);
void        bk_display_layer_name(int x, int y, int layer, painter_font_handle_t font);
int         bk_display_layer_info(int x, int y, int layer, painter_font_handle_t font, bool rewrite_all);

int bk_display_info_base(uint16_t x, uint16_t y, painter_font_handle_t font_on, painter_font_handle_t font_off, bool render_all);
int bk_display_info_pointer(uint16_t x, uint16_t y, painter_font_handle_t font_on, painter_font_handle_t font_off, bool render_all);
int bk_display_info_media(uint16_t x, uint16_t y, painter_font_handle_t font_on, painter_font_handle_t font_off, bool render_all);

static const char *layer_strings[] = {
    "00 BASE",
    "01 FUNCT",
    "02 NAV",
    "03 MED/RGB",
    "04 POINT",
    "05 NUM",
    "06 SYM"
};

// static const hsv_t colors[] = {
//     [LAYER_BASE]       = {HSV_WHITE},
//     [LAYER_FUNCTION]   = {HSV_BLUE},
//     [LAYER_NAVIGATION] = {HSV_AZURE},
//     [LAYER_MEDIA]      = {HSV_ORANGE},
//     [LAYER_POINTER]    = {HSV_GREEN},
//     [LAYER_NUMERAL]    = {HSV_TEAL},
//     [LAYER_SYMBOLS]    = {HSV_PURPLE}
// };