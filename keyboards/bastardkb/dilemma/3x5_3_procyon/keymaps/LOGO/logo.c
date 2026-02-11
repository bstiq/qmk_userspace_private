#include "logo.h"
#include "logo_data.h"  // Nuestro array de pixeles

// Función que copia la imagen al framebuffer
void logo_draw(uint16_t *fb, int width, int height) {
    for (int y = 0; y < logo_h; y++) {
        for (int x = 0; x < logo_w; x++) {
            fb[y * width + x] = logo_img[y * logo_w + x];
        }
    }
}

