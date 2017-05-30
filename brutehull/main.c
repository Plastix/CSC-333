#include <time.h>
#include <printf.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "ppm.h"
#include "omp.h"
#include "ppm_utils.h"

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point *points;
    int length;
} PointList;


// Constructor for creating a new PointList
// Allocates space on the heap so use freePointList(list) after
PointList *pointList(int n) {
    PointList *result = calloc(1, sizeof(PointList));
    result->length = n;
    result->points = calloc((size_t) n, sizeof(Point));
    return result;
}

// Destructor for PointLists
void freePointList(PointList *pointList) {
    free(pointList->points);
    free(pointList);
};

// Useful debugging commands
//void printPoint(Point point) {
//    printf("(%i, %i)\n", point.x, point.y);
//}
//
//void printPointList(PointList pointList, char *label) {
//    printf("--- %s ---\n", label);
//    int i;
//    for (i = 0; i < pointList.length; i++) {
//        printPoint(pointList.points[i]);
//    }
//}

int ccw(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

PointList *convexHull(PointList *list, int threads) {
    printf("Calculating convex hull...\n");
    int n = list->length;

    Point *hull = calloc((size_t) (2 * n), sizeof(Point));
    int index = 0;
    int x;

#pragma omp parallel for num_threads(threads)
    for (x = 0; x < n; x++) {
        Point a = list->points[x];
        int y;
        for (y = 0; y < n; y++) {
            Point b = list->points[y];

            if (a.x == b.x && a.y == b.y) {
                continue;
            }

            int isConvex = 1;
            int k;
            for (k = 0; k < n; k++) {
                Point c = list->points[k];

                if ((a.x == c.x && a.y == c.y) || (b.x == c.x && b.y == c.y)) {
                    continue;
                }

                if (ccw(a, b, c) < 0) {
                    isConvex = 0;
                    break;
                }
            }

            if (isConvex) {
#pragma omp critical
                {
                    hull[index++] = a;
                    hull[index++] = b;
                };
            }
        }
    }

    PointList *result = pointList(index);
    memcpy(result->points, hull, (index) * sizeof(Point));
    free(hull);

    return result;
}

void drawHull(ppm_image *image, PointList *hull) {
    printf("Drawing hull...\n");
    int i;
    for (i = 0; i < hull->length - 1; i += 2) {
        Point one = hull->points[i];
        Point two = hull->points[i + 1];
        draw_circle(image, one.x, one.y, 10, 255, 0, 255);
        draw_circle(image, two.x, two.y, 10, 255, 0, 255);
        line(image, one.x, one.y, two.x, two.y, 0, 0, 255);
    }

}

PointList *generatePoints(int numPoints, int w, int h) {
    printf("Generating points...\n");
    PointList *points = pointList(numPoints);
    int i = 0;
    while (i < numPoints) {
        int x = (rand() % w);
        int y = (rand() % h);

        if (hypot(x - (w / 2.0), y - (h / 2.0)) <= 0.4 * w) {
            points->points[i].x = x;
            points->points[i].y = y;
            i++;
        }

    }

    return points;
}

void drawPoints(ppm_image *image, PointList *points) {
    int i;
    for (i = 0; i < points->length; i++) {
        Point point = points->points[i];
        draw_circle(image, point.x, point.y, 2, 0, 0, 0);
    }
}


int main(int argc, char *argv[]) {
    unsigned int seed = (unsigned int) time(NULL);
    srand(seed);

    if (argc != 6) {
        printf("Usage: %s <width> <height> <num points> <thread number> <output file name>\n", argv[0]);
        exit(1);
    }

    int w = atoi(argv[1]);
    int h = atoi(argv[2]);

    // Setup image
    ppm_image *image = make_image(w, h);
    set_background_color(image, 255, 255, 255);

    int numPoints = atoi(argv[3]);
    if (numPoints < 2) {
        printf("Number of points must be at least 2!");
        exit(1);
    }

    printf("Random Seed: %i\n", seed);

    // Generate random points and draw points on image
    PointList *points = generatePoints(numPoints, w, h);
    drawPoints(image, points);

    // Calculate convex hull
    int threads = atoi(argv[4]);
    double start = omp_get_wtime();
    PointList *hull = convexHull(points, threads);
    double end = omp_get_wtime();
    double elapsed = end - start;

    // Draw convex hull on image
    drawHull(image, hull);

    printf("Elapsed Time: %e\n", elapsed);

    // Free resources and save image
    freePointList(points);
    freePointList(hull);
    write_image(image, argv[5]);
    free_image(image);

    return 0;

}
