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

/**
 * Basic implementation of Bresenham's line algorithm
 */
void draw_line(ppm_image *image, int x0, int y0, int x1, int y1, int r, int g, int b) {
    double deltax = x1 - x0;
    double deltay = y1 - y0;
    double deltaerr = fabs(deltay / deltax);

    int x, y = y0;
    // Special case: Vertical line
    if (x1 - x0 == 0) {
        for (; y < y1; y++) {
            set_pixel(image, x0, y, r, g, b);
        }
    } else {
        double error = deltaerr - 0.5;

        for (x = x0; x <= x1; x++) {
            set_pixel(image, x, y, r, g, b);
            error = error + deltaerr;

            if (error >= 0.5) {
                y++;
                error--;
            }
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