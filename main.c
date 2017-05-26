
#include "ppm.h"

int main(int argc, char *argv[]) {

    ppm_image *image = make_image(500, 500);
    int w = image->width;
    int h = image->height;

    int i, j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            int color = 255;

            if (i == h / 2 || j == w / 2) color = 127;
            set_pixel(image, i, j, color, color, color);
        }
    }

    write_image(image, "test.ppm");
    free_image(image);

    return 0;

}