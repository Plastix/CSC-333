#include <time.h>
#include <printf.h>
#include <string.h>
#include "ppm.h"
#include "ppm_utils.h"

const int w = 500;
const int h = 500;

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
    double area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    if (area < 0) {
        return -1;
    } else if (area > 0) {
        return 1;
    }
    return 0;
}

PointList convexHull(PointList pointList) {
    int n = pointList.length;

    Point hull[n];
    int index = 0;

    int x;
    for (x = 0; x < n; x++) {
        int y;
        for (y = 0; y < n; y++) {
            if (x == y) {
                continue;
            }

            Point a = pointList.points[x];
            Point b = pointList.points[y];

            int isConvex = 1;
            int k;
            for (k = 0; k < n; k++) {

                if (k == x || k == y) {
                    continue;
                }

                Point c = pointList.points[k];

                if (ccw(a, b, c) == -1) {
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

    ppm_image *image = make_image(w, h);
    set_background_color(image, 255, 255, 255);

    PointList points;
    points.length = 20;
    points.points = calloc((size_t) points.length, sizeof(Point));

    int i;
    for (i = 0; i < points.length; i++) {
        int x = (rand() % 200) + 150;
        int y = (rand() % 200) + 150;
        points.points[i].x = x;
        points.points[i].y = y;
        draw_circle(image, x, y, 5, 0, 0, 0);
    }

    PointList hull = convexHull(points);

    for (i = 0; i < hull.length - 1; i += 2) {
        Point one = hull.points[i];
        Point two = hull.points[i + 1];
        draw_circle(image, one.x, one.y, 10, 255, 0, 255);
        draw_circle(image, two.x, two.y, 10, 255, 0, 255);

//        printf("Pair\n");
//        printPoint(one);
//        printPoint(two);
 
        line(image, one.x, one.y, two.x, two.y, 0, 0, 255);
    }

    free(points.points);
    write_image(image, "test.ppm");
    free_image(image);

    return 0;

}