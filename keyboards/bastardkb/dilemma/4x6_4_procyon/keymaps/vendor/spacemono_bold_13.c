/*******************************************************************************
 * Size: 10 px
 * Bpp: 2
 * Opts: --bpp 2 --size 10 --no-compress --stride 1 --align 1 --font SpaceMono-Bold.ttf --symbols ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz :-0123456789 --format lvgl -o spacemono_bold_13.c
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



#ifndef SPACEMONO_BOLD_13
#define SPACEMONO_BOLD_13 1
#endif

#if SPACEMONO_BOLD_13

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+002D "-" */
    0x7e, 0x0,

    /* U+0030 "0" */
    0x1f, 0x83, 0x5d, 0x75, 0xab, 0xaa, 0x70, 0xa7,
    0x5d, 0x1f, 0x80,

    /* U+0031 "1" */
    0x3f, 0x3, 0xb0, 0x77, 0x0, 0x70, 0x7, 0x0,
    0xb0, 0x7f, 0xe0,

    /* U+0032 "2" */
    0x1f, 0x83, 0x5d, 0x70, 0xe0, 0x7c, 0x3d, 0x7,
    0x40, 0x7f, 0xe0,

    /* U+0033 "3" */
    0x7f, 0xc0, 0x6c, 0xb, 0x40, 0xb8, 0x20, 0xe7,
    0x5d, 0x1f, 0x80,

    /* U+0034 "4" */
    0xf, 0xc2, 0xec, 0x36, 0xca, 0x2c, 0xbf, 0xf0,
    0x2c, 0x2, 0xc0,

    /* U+0035 "5" */
    0x7f, 0xd7, 0x0, 0x77, 0xc7, 0xde, 0x10, 0xb7,
    0x5e, 0x2f, 0x80,

    /* U+0036 "6" */
    0x1f, 0x87, 0x59, 0x7b, 0x87, 0x5d, 0x70, 0xe7,
    0x5d, 0x1f, 0x80,

    /* U+0037 "7" */
    0x7f, 0xe0, 0xe, 0x1, 0xc0, 0x38, 0xf, 0x1,
    0xc0, 0x38, 0x0,

    /* U+0038 "8" */
    0x1f, 0x83, 0x5d, 0x34, 0xd2, 0xf8, 0x75, 0xd7,
    0x5e, 0x2f, 0x80,

    /* U+0039 "9" */
    0x1f, 0x87, 0x5d, 0x70, 0xe3, 0xfe, 0x10, 0xe3,
    0x5d, 0x1f, 0x80,

    /* U+003A ":" */
    0xb2, 0x80, 0x28, 0xb0,

    /* U+0041 "A" */
    0x1f, 0x42, 0xe8, 0x39, 0xc3, 0x4c, 0x7f, 0xd7,
    0xa, 0xa0, 0xb0,

    /* U+0042 "B" */
    0xbf, 0xc3, 0x9e, 0x34, 0xd3, 0xf8, 0x39, 0xd3,
    0x9e, 0xbf, 0xc0,

    /* U+0043 "C" */
    0x1f, 0x83, 0x5d, 0x70, 0x5b, 0x0, 0x70, 0x57,
    0x5d, 0x1f, 0x80,

    /* U+0044 "D" */
    0xbf, 0x83, 0x9d, 0x34, 0xe3, 0x4e, 0x34, 0xe3,
    0x9d, 0xbf, 0x80,

    /* U+0045 "E" */
    0x3f, 0xc3, 0x40, 0x30, 0x3, 0xfc, 0x34, 0x3,
    0x40, 0x3f, 0xd0,

    /* U+0046 "F" */
    0x3f, 0xd3, 0x40, 0x30, 0x3, 0xfd, 0x34, 0x3,
    0x0, 0x30, 0x0,

    /* U+0047 "G" */
    0x2f, 0x87, 0x5d, 0xb0, 0xb, 0x7e, 0xb0, 0xe7,
    0x5e, 0x2f, 0xa0,

    /* U+0048 "H" */
    0x70, 0xe7, 0xe, 0x70, 0xe7, 0xfe, 0x70, 0xe7,
    0xe, 0x70, 0xe0,

    /* U+0049 "I" */
    0x7f, 0xd0, 0xb0, 0xa, 0x0, 0xa0, 0xa, 0x0,
    0xb0, 0x7f, 0xd0,

    /* U+004A "J" */
    0x3, 0xf0, 0x1d, 0x0, 0xc0, 0xc, 0x70, 0xc7,
    0x6c, 0x2f, 0x40,

    /* U+004B "K" */
    0x30, 0xd3, 0x2c, 0x37, 0x3, 0xe0, 0x3b, 0x3,
    0x2c, 0x30, 0xd0,

    /* U+004C "L" */
    0x30, 0x3, 0x0, 0x30, 0x3, 0x0, 0x30, 0x3,
    0x40, 0x3f, 0xd0,

    /* U+004D "M" */
    0xf9, 0xfe, 0xaf, 0xea, 0xbe, 0xeb, 0xef, 0xbd,
    0xf7, 0xdf, 0x70,

    /* U+004E "N" */
    0x7d, 0xe7, 0xde, 0x7a, 0xe7, 0xae, 0x77, 0xe7,
    0x7e, 0x73, 0xe0,

    /* U+004F "O" */
    0x1f, 0x83, 0x5d, 0x70, 0xab, 0xa, 0x70, 0xa7,
    0x5d, 0x1f, 0x80,

    /* U+0050 "P" */
    0x7f, 0x87, 0x5d, 0x70, 0xe7, 0xfc, 0x70, 0x7,
    0x0, 0x70, 0x0,

    /* U+0051 "Q" */
    0x1f, 0x83, 0x5d, 0x70, 0xab, 0xa, 0xb0, 0xa7,
    0xe, 0x3f, 0xc0, 0xf0, 0x7, 0x80, 0x0,

    /* U+0052 "R" */
    0x7f, 0x87, 0x5e, 0x70, 0xd7, 0xfc, 0x70, 0xd7,
    0xd, 0x70, 0xd0,

    /* U+0053 "S" */
    0x2f, 0x87, 0x5d, 0x74, 0x51, 0xfc, 0x60, 0xe7,
    0x5e, 0x2f, 0x80,

    /* U+0054 "T" */
    0x7f, 0xe0, 0xb0, 0xa, 0x0, 0xa0, 0xa, 0x0,
    0xa0, 0xa, 0x0,

    /* U+0055 "U" */
    0x70, 0xa7, 0xa, 0x70, 0xa7, 0xa, 0x70, 0xe7,
    0x5d, 0x2f, 0x80,

    /* U+0056 "V" */
    0xa0, 0xb7, 0xe, 0x30, 0xd3, 0x4c, 0x39, 0xc2,
    0xe8, 0x1f, 0x40,

    /* U+0057 "W" */
    0xdf, 0x73, 0x7d, 0xce, 0xfb, 0x3b, 0xac, 0xee,
    0xb2, 0xaa, 0xcb, 0xaf, 0x0,

    /* U+0058 "X" */
    0x70, 0xe3, 0x4c, 0x2a, 0xc1, 0xf4, 0x2e, 0x83,
    0x5c, 0x70, 0xe0,

    /* U+0059 "Y" */
    0xa0, 0xb1, 0xc3, 0x43, 0x9c, 0x7, 0xd0, 0xf,
    0x0, 0x28, 0x0, 0xa0, 0x0,

    /* U+005A "Z" */
    0x7f, 0xe0, 0x1e, 0x2, 0xc0, 0xf0, 0x38, 0xb,
    0x40, 0xbf, 0xe0,

    /* U+0061 "a" */
    0x2f, 0xd7, 0x5d, 0xa0, 0xd7, 0x6e, 0x2e, 0xb0,

    /* U+0062 "b" */
    0x70, 0x7, 0x0, 0x7b, 0x87, 0x5e, 0x70, 0xa7,
    0x5e, 0x7b, 0x80,

    /* U+0063 "c" */
    0x1f, 0x87, 0x5d, 0x70, 0x7, 0x5d, 0x1f, 0x80,

    /* U+0064 "d" */
    0x0, 0xa0, 0xa, 0x2f, 0xe7, 0x5e, 0xa0, 0xa7,
    0x5e, 0x2e, 0xa0,

    /* U+0065 "e" */
    0x1f, 0x83, 0x4d, 0x7f, 0xe3, 0x58, 0x1f, 0x80,

    /* U+0066 "f" */
    0xb, 0xc0, 0xe0, 0x7f, 0xd0, 0xe0, 0xd, 0x0,
    0xd0, 0xd, 0x0,

    /* U+0067 "g" */
    0x2e, 0xa7, 0x5e, 0xa0, 0xa7, 0x5e, 0x2f, 0xe0,
    0xe, 0x2f, 0xd0,

    /* U+0068 "h" */
    0x30, 0x3, 0x0, 0x37, 0xc3, 0x4d, 0x30, 0xd3,
    0xd, 0x30, 0xd0,

    /* U+0069 "i" */
    0xb, 0x0, 0x60, 0x3f, 0x0, 0xb0, 0x7, 0x0,
    0xb0, 0x3f, 0xd0,

    /* U+006A "j" */
    0xe, 0xd, 0xbd, 0xd, 0xd, 0xd, 0xd, 0xd,
    0xfd,

    /* U+006B "k" */
    0xd0, 0x34, 0xd, 0x73, 0xb4, 0xfc, 0x3b, 0x4d,
    0x70,

    /* U+006C "l" */
    0x3f, 0x0, 0xb0, 0xb, 0x0, 0xb0, 0xb, 0x0,
    0xb0, 0x7f, 0xd0,

    /* U+006D "m" */
    0xbe, 0xda, 0xbb, 0xaa, 0xba, 0xab, 0xaa, 0xb0,

    /* U+006E "n" */
    0x3b, 0x83, 0x4d, 0x30, 0xd3, 0xd, 0x30, 0xd0,

    /* U+006F "o" */
    0x2f, 0x87, 0x5d, 0x70, 0xa7, 0x5d, 0x1f, 0x80,

    /* U+0070 "p" */
    0x7b, 0x87, 0xe, 0x70, 0xa7, 0x5e, 0x7f, 0x87,
    0x0, 0x70, 0x0,

    /* U+0071 "q" */
    0x2e, 0xa7, 0x5e, 0xa0, 0xa7, 0x5e, 0x2f, 0xe0,
    0xa, 0x0, 0xa0,

    /* U+0072 "r" */
    0x7f, 0xd2, 0xde, 0x28, 0x2, 0xc0, 0xbf, 0x0,

    /* U+0073 "s" */
    0x1f, 0x43, 0x94, 0x1f, 0x82, 0x5c, 0x1f, 0x80,

    /* U+0074 "t" */
    0xd, 0x0, 0xd0, 0x7f, 0xc0, 0xd0, 0xd, 0x0,
    0xe0, 0xb, 0xc0,

    /* U+0075 "u" */
    0x30, 0xd3, 0xd, 0x30, 0xd3, 0x5d, 0x2e, 0xd0,

    /* U+0076 "v" */
    0x70, 0xe3, 0xc, 0x35, 0xc2, 0xa8, 0x1f, 0x40,

    /* U+0077 "w" */
    0xdf, 0x79, 0xf7, 0xae, 0xba, 0xea, 0xba, 0xe0,

    /* U+0078 "x" */
    0x34, 0xc2, 0xa8, 0xf, 0x42, 0xe8, 0x34, 0xd0,

    /* U+0079 "y" */
    0x30, 0xd3, 0xd, 0x30, 0xd3, 0x5d, 0x2e, 0xd0,
    0xd, 0x1f, 0xd0,

    /* U+007A "z" */
    0x3f, 0xc0, 0x2c, 0x1f, 0x43, 0x80, 0x3f, 0xc0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 98, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 98, .box_w = 4, .box_h = 2, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 2, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 13, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 24, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 35, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 46, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 57, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 68, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 79, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 90, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 101, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 112, .adv_w = 98, .box_w = 3, .box_h = 5, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 116, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 127, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 138, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 149, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 160, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 171, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 182, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 204, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 215, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 226, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 237, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 248, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 259, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 270, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 292, .adv_w = 98, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 307, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 318, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 329, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 340, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 351, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 362, .adv_w = 98, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 375, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 386, .adv_w = 98, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 399, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 410, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 418, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 429, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 437, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 448, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 456, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 467, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 478, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 489, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 500, .adv_w = 98, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 509, .adv_w = 98, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 518, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 529, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 537, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 545, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 553, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 564, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 575, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 583, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 591, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 602, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 610, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 618, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 626, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 634, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 645, .adv_w = 98, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0xd
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 14, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 2, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    },
    {
        .range_start = 48, .range_length = 11, .glyph_id_start = 3,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 14,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 26, .glyph_id_start = 40,
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
    .cmap_num = 4,
    .bpp = 2,
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
const lv_font_t spacemono_bold_13 = {
#else
lv_font_t spacemono_bold_13 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 10,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
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



#endif /*#if SPACEMONO_BOLD_13*/
