/*******************************************************************************
 * Size: 15 px
 * Bpp: 1
 * Opts: --bpp 1 --size 15 --no-compress --stride 1 --align 1 --font Montserrat-Bold.ttf --symbols ABCDEFGHIJKLMNOPQRSTUVWXYZ --format lvgl -o montserrat.c
 ******************************************************************************/

#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif



#ifndef MONTSERRAT
#define MONTSERRAT 1
#endif

#if MONTSERRAT

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0041 "A" */
    0xe, 0x0, 0xf0, 0x1f, 0x1, 0xb8, 0x39, 0x83,
    0x1c, 0x7f, 0xc7, 0xfc, 0x60, 0xee, 0x6,

    /* U+0042 "B" */
    0xff, 0x7f, 0xf0, 0x78, 0x3f, 0xf7, 0xff, 0x7,
    0x83, 0xff, 0xff, 0x80,

    /* U+0043 "C" */
    0x1f, 0x1f, 0xe6, 0x1b, 0x0, 0xc0, 0x30, 0xc,
    0x1, 0x86, 0x7f, 0x87, 0xc0,

    /* U+0044 "D" */
    0xfe, 0x3f, 0xec, 0x3b, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0xe, 0xff, 0xbf, 0x80,

    /* U+0045 "E" */
    0xff, 0xff, 0xc0, 0xc0, 0xff, 0xff, 0xc0, 0xc0,
    0xff, 0xff,

    /* U+0046 "F" */
    0xff, 0xff, 0xe0, 0xe0, 0xff, 0xff, 0xe0, 0xe0,
    0xe0, 0xe0,

    /* U+0047 "G" */
    0x1f, 0xf, 0xf7, 0xb, 0x0, 0xc0, 0x30, 0x3c,
    0xd, 0xc3, 0x3f, 0xc7, 0xe0,

    /* U+0048 "H" */
    0xe1, 0xf0, 0xf8, 0x7c, 0x3f, 0xff, 0xff, 0x87,
    0xc3, 0xe1, 0xf0, 0xc0,

    /* U+0049 "I" */
    0xff, 0xff, 0xf0,

    /* U+004A "J" */
    0x7e, 0xfc, 0x18, 0x30, 0x60, 0xc1, 0xa3, 0xfe,
    0x78,

    /* U+004B "K" */
    0xc3, 0xb0, 0xcc, 0x63, 0x30, 0xd8, 0x3f, 0xe,
    0xe3, 0x1c, 0xc3, 0xb0, 0x70,

    /* U+004C "L" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xff, 0xff,

    /* U+004D "M" */
    0xc0, 0x3e, 0x7, 0xf0, 0xff, 0xf, 0xd9, 0xbd,
    0xfb, 0xcf, 0x3c, 0xe3, 0xc6, 0x3c, 0x3,

    /* U+004E "N" */
    0xc1, 0xf0, 0xfc, 0x7f, 0x3f, 0xde, 0xef, 0x3f,
    0x8f, 0xc3, 0xe0, 0xc0,

    /* U+004F "O" */
    0x1f, 0xf, 0xf1, 0xc3, 0x60, 0x3c, 0x7, 0x80,
    0xf0, 0x1b, 0xe, 0x7f, 0xc3, 0xe0,

    /* U+0050 "P" */
    0xfe, 0x7f, 0xb0, 0xf8, 0x3c, 0x3f, 0xfb, 0xf9,
    0x80, 0xc0, 0x60, 0x0,

    /* U+0051 "Q" */
    0x1f, 0x7, 0xfc, 0x61, 0xcc, 0x6, 0xc0, 0x6c,
    0x6, 0xc0, 0x66, 0x1c, 0x7f, 0xc1, 0xf0, 0x7,
    0xe0, 0x1e,

    /* U+0052 "R" */
    0xfe, 0x7f, 0xb0, 0xf8, 0x3c, 0x3f, 0xfb, 0xf9,
    0x8e, 0xc3, 0x60, 0xc0,

    /* U+0053 "S" */
    0x3e, 0xfe, 0xc2, 0xc0, 0x78, 0x1e, 0x3, 0x83,
    0xff, 0x7c,

    /* U+0054 "T" */
    0xff, 0xff, 0xc3, 0x1, 0x80, 0xc0, 0x60, 0x30,
    0x18, 0xc, 0x6, 0x0,

    /* U+0055 "U" */
    0xc1, 0xe0, 0xf0, 0x78, 0x3c, 0x1e, 0xf, 0x7,
    0xc7, 0x7f, 0x1f, 0x0,

    /* U+0056 "V" */
    0xe0, 0x6c, 0x1d, 0xc3, 0x38, 0xe3, 0x18, 0x77,
    0x6, 0xc0, 0xf8, 0xe, 0x1, 0xc0,

    /* U+0057 "W" */
    0xe1, 0xc1, 0xb0, 0xe1, 0xdc, 0x78, 0xc6, 0x7c,
    0x63, 0x36, 0x71, 0xd9, 0xb0, 0x7c, 0xd8, 0x3c,
    0x7c, 0x1e, 0x1c, 0x7, 0xe, 0x0,

    /* U+0058 "X" */
    0x70, 0xc7, 0x38, 0x6e, 0xf, 0x80, 0xe0, 0x1e,
    0x7, 0xc1, 0xdc, 0x31, 0xce, 0x1c,

    /* U+0059 "Y" */
    0xe1, 0xd8, 0x67, 0x38, 0xec, 0x1e, 0x7, 0x80,
    0xc0, 0x30, 0xc, 0x3, 0x0,

    /* U+005A "Z" */
    0xff, 0xff, 0xc1, 0xc1, 0xc1, 0xc0, 0xe0, 0xe0,
    0xe0, 0xff, 0xff, 0xc0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 184, .box_w = 12, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 15, .adv_w = 184, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 27, .adv_w = 174, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 40, .adv_w = 198, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 53, .adv_w = 161, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 63, .adv_w = 153, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 73, .adv_w = 185, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 86, .adv_w = 194, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 98, .adv_w = 79, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 101, .adv_w = 130, .box_w = 7, .box_h = 10, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 110, .adv_w = 178, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 123, .adv_w = 145, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 133, .adv_w = 229, .box_w = 12, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 148, .adv_w = 194, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 160, .adv_w = 203, .box_w = 11, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 174, .adv_w = 176, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 186, .adv_w = 203, .box_w = 12, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 204, .adv_w = 176, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 216, .adv_w = 153, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 226, .adv_w = 148, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 238, .adv_w = 189, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 250, .adv_w = 179, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 264, .adv_w = 279, .box_w = 17, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 286, .adv_w = 171, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 300, .adv_w = 162, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 313, .adv_w = 161, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif

};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t montserrat = {
#else
lv_font_t montserrat = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 12,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if MONTSERRAT*/
