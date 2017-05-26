#include <time.h>
#include "ppm.h"
#include "ppm_utils.h"

typedef struct {
    int x, y;
} Point;

int main(int argc, char *argv[]) {

    srand((unsigned int) time(NULL));

    int w = 500;
    int h = 500;
    ppm_image *image = make_image(w, h);
    set_background_color(image, 255, 255, 255);

    draw_line(image, 0, h / 2, w, h / 2, 255, 0, 0);
    draw_line(image, w / 2, 0, w / 2, h, 0, 255, 0);

    int numPoints = 20;
    Point points[numPoints];
    int i;
    for (i = 0; i < numPoints; i++) {
        int x = rand() % 500;
        int y = rand() % 500;
        points[i].x = x;
        points[i].y = y;
        draw_circle(image, x, y, 5, 0, 0, 0);
    }

    write_image(image, "test.ppm");
    free_image(image);

    return 0;

}