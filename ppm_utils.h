#include "ppm.h"

void set_background_color(ppm_image *image, int r, int g, int b) {
    int w = image->width;
    int h = image->height;

    int i, j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            set_pixel(image, i, j, r, g, b);
        }
    }
}