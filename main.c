#include <time.h>
#include <printf.h>
#include <string.h>
#include "ppm.h"
#include "ppm_utils.h"

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point *points;
    int length;
} PointList;

void printPoint(Point point) {
    printf("(%i, %i)\n", point.x, point.y);
}

void printPointList(PointList pointList) {
    printf("--- Set ---\n");
    int i;
    for (i = 0; i < pointList.length; i++) {
        printPoint(pointList.points[i]);
    }
}

int ccw(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

PointList convexHull(PointList pointList) {
    int n = pointList.length;

    Point hull[n];
    int index = 0;

    int x;
    for (x = 0; x < n; x++) {
        int y;
        for (y = 0; y < n; y++) {
            Point a = pointList.points[x];
            Point b = pointList.points[y];

            if (a.x == b.x && a.y == b.y) {
                continue;
            }

            int isConvex = 1;
            int k;
            for (k = 0; k < n; k++) {
                Point c = pointList.points[k];

                if ((a.x == c.x && a.y == c.y) || (b.x == c.x && b.y == c.y)) {
                    continue;
                }

                if (ccw(a, b, c) < 0) {
                    isConvex = 0;
                    break;
                }
            }

            if (isConvex) {
                hull[index] = a;
                index++;
                hull[index] = b;
                index++;
            }
        }
    }

    PointList result;
    result.length = index;
    result.points = calloc((size_t) index, sizeof(Point));
    memcpy(result.points, hull, index * sizeof(Point));

    return result;
}

int main(int argc, char *argv[]) {

    srand((unsigned int) time(NULL));

    if (argc != 4) {
        printf("Usage: %s <width> <height> <num points>\n", argv[0]);
        exit(1);
    }

    int w = atoi(argv[1]);
    int h = atoi(argv[2]);

    ppm_image *image = make_image(w, h);
    set_background_color(image, 255, 255, 255);

    PointList points;
    points.length = atoi(argv[3]);
    points.points = calloc((size_t) points.length, sizeof(Point));

    printf("Generating points...\n");
    int i = 0;
    while (i < points.length) {
        int x = (rand() % w);
        int y = (rand() % h);

        if (hypot(x - (w / 2.0), y - (h / 2.0)) <= 0.4 * w) {
            points.points[i].x = x;
            points.points[i].y = y;
            draw_circle(image, x, y, 1, 0, 0, 0);
            i++;
        }

    }

    printf("Calculating convex hull...\n");
    PointList hull = convexHull(points);

    printf("Drawing hull...\n");
    for (i = 0; i < hull.length - 1; i += 2) {
        Point one = hull.points[i];
        Point two = hull.points[i + 1];
        draw_circle(image, one.x, one.y, 10, 255, 0, 255);
        draw_circle(image, two.x, two.y, 10, 255, 0, 255);
        line(image, one.x, one.y, two.x, two.y, 0, 0, 255);
    }

    free(points.points);
    write_image(image, "test.ppm");
    free_image(image);

    return 0;

}