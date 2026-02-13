#include "logo.h"

void logo_draw(uint16_t *fb, int width, int height) {
    for (int y = 0; y < LOGO_H; y++) {
        for (int x = 0; x < LOGO_W; x++) {
            fb[y * width + x] = logo_data[y * LOGO_W + x];
        }
    }
}