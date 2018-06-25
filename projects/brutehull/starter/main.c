#include <time.h>
#include <printf.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ppm.h"
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

// TODO Implement me!!
PointList *convexHull(PointList *list, int threads) {

    // Calculate the convex hull of the specified list of points
    // You will need to create a NEW PointList with points on the hull and return it
    // This requires that you know how many points are on the hull!


    // Return an empty point list so the starter code will compile
    // Change this!
    return pointList(0);
}

// TODO Implement me!!
void drawHull(ppm_image *image, PointList *hull) {
    // Highlight points on the hull by drawing circles around them
    // Draw lines connecting points along the hull
    // This will have to be tailored to the specific order of points you return in convexHull(...)
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
    PointList *hull = convexHull(points, threads);

    // Draw convex hull on image
    drawHull(image, hull);

    // Free resources and save image
    freePointList(points);
    freePointList(hull);
    write_image(image, argv[5]);
    free_image(image);

    return 0;

}
