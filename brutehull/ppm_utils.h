#include <stdlib.h>
#include <math.h>
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

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// From
// https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham%E2%80%99s-Line-Drawing-Algorithm
void line(ppm_image *image, int x0, int y0, int x1, int y1, int r, int g, int b) {
    int steep = 0;
    if (abs(x0 - x1) < abs(y0 - y1)) { // if the line is steep, we transpose the image
        swap(&x0, &y0);
        swap(&x1, &y1);
        steep = 1;
    }
    if (x0 > x1) { // make it left−to−right
        swap(&x0, &x1);
        swap(&y0, &y1);
    }
    for (int x = x0; x <= x1; x++) {
        float t = (x - x0) / (float) (x1 - x0);
        int y = (int) (y0 * (1.0 - t) + y1 * t);
        if (steep) {
            set_pixel(image, y, x, r, g, b);
        } else {
            set_pixel(image, x, y, r, g, b);
        }
    }
}


/**
 * Midpoint circle algorithm
 * Modified code from Wikipedia
 */
void draw_circle(ppm_image *image, int x0, int y0, int radius, int r, int g, int b) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        set_pixel(image, x0 + x, y0 + y, r, g, b);
        set_pixel(image, x0 + y, y0 + x, r, g, b);
        set_pixel(image, x0 - y, y0 + x, r, g, b);
        set_pixel(image, x0 - x, y0 + y, r, g, b);
        set_pixel(image, x0 - x, y0 - y, r, g, b);
        set_pixel(image, x0 - y, y0 - x, r, g, b);
        set_pixel(image, x0 + y, y0 - x, r, g, b);
        set_pixel(image, x0 + x, y0 - y, r, g, b);

        y += 1;
        if (err <= 0) {
            err += 2 * y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}