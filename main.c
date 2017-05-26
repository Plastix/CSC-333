#include <stdio.h>

#include "ppm.h"

int main(int argc, char *argv[]) {

    int w = 1024, h = 768;

    initialize(w, h);

    int i, j;
    for (i = 0; i < w; i++) {
        set_pixel_rgb(i, h / 2, 0, 255, 0);
    }

    for (j = 0; j < h; j++) {
        set_pixel_rgb(w / 2, j, 255, 0, 0);
    }
    save_image("test_set_bg_pixel.ppm");

    return 0;

}