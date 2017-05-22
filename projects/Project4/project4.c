/*  project4.c
by John Rieffel, Union College

based initially on demopgm.c  by Richard Zanibbi (May 1998) for
 * CISC 859 at Queen's University.
 *

 Description: shows a simple sobel-Filter convolution of a pgm image
 */

#include "PGM.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

typedef struct {
    int votes;
    int radius;
    int theta;

} Line;

int line_compare(const void *a, const void *b) {
    Line *x = (Line *) a;
    Line *y = (Line *) b;
    return y->votes - x->votes;
}

void printLine(Line line) {
    printf("[votes: %i, radius: %i, theta: %i]\n", line.votes, line.radius, line.theta);
}

void edgeGradient(unsigned char inImg[MAXROWS][MAXCOLS], int rows, int cols,
                  unsigned char outimg[MAXROWS][MAXCOLS], int thread_num) {
    int sobelXkernel[3][3] = {{1, 0, -1},
                              {2, 0, -2},
                              {1, 0, -1}};
    int sobelYkernel[3][3] = {{1,  2,  1},
                              {0,  0,  0},
                              {-1, -2, -1}};

    int row, col;
#pragma omp parallel for num_threads(thread_num)
    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {

            double xAcc = 0;
            double yAcc = 0;
            //instead of wrapping around a toroid we just repeat first/last coloum and row
            int krow, kcol;

            for (krow = 0; krow < 3; krow++) {
                for (kcol = 0; kcol < 3; kcol++) {
                    //get the kernel value
                    int xval = sobelXkernel[krow][kcol];
                    int yval = sobelYkernel[krow][kcol];

                    int krowoffset = krow - 1;
                    int kcoloffset = kcol - 1;
                    int currow = row + krowoffset;
                    int curcol = col + kcoloffset;

                    currow = currow < 0 ? 0 : currow;
                    curcol = curcol < 0 ? 0 : curcol;
                    currow = currow > rows - 1 ? rows - 1 : currow;
                    curcol = curcol > cols - 1 ? cols - 1 : curcol;
                    xAcc += inImg[currow][curcol] * 0.25 * xval;
                    yAcc += inImg[currow][curcol] * 0.25 * yval;
                }
            }

            outimg[row][col] = (unsigned char) sqrt(pow(xAcc, 2) + pow(yAcc, 2));
        }
    }
}

Line *lineDetect(unsigned char inImg[MAXROWS][MAXCOLS], int rows, int cols,
                 int thetaBins, int radiiBins, int thread_num,
                 unsigned char threshold, unsigned char numLines) {


    long numBins = thetaBins * radiiBins;
    int counts[radiiBins][thetaBins];
    memset(counts, 0, numBins * sizeof(int)); // Zero out counts

    double maxRadius = hypot(cols - 1, rows - 1);
    double radiiBinWidth = maxRadius / radiiBins;
    double thetaBinWidth = 180.0 / thetaBins;

    int row, col;
    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {
            unsigned char pixel = inImg[row][col];

            if (pixel >= threshold) {
                int theta;
                for (theta = 0; theta < 180; theta += thetaBinWidth) {
                    double rad = degreesToRadians(theta);
                    double r = col * cos(rad) + row * sin(rad);
                    int radiiBin = (int) floor(fabs(r) / radiiBinWidth);
                    int thetaBin = (int) floor(theta / thetaBinWidth);

                    counts[radiiBin][thetaBin]++;
                }
            }
        }
    }

    // Convert tally array to flat array of Line objects
    Line *lines = calloc((size_t) numBins, sizeof(Line));
    int i = 0;
    int t, r;
    for (r = 0; r < radiiBins; r++) {
        for (t = 0; t < thetaBins; t++) {
            lines[i].votes = counts[r][t];
            lines[i].theta = (int) floor(t * thetaBinWidth);
            lines[i].radius = (int) floor(r * radiiBinWidth);
            i++;
        }
    }

    // Sort line array by number of votes
    qsort(lines, (size_t) numBins, sizeof(Line), line_compare);

    Line *result = calloc(numLines, sizeof(Line));
    memcpy(result, lines, numLines * sizeof(Line));
    free(lines);

    return result;
}

void main(int argc, char *argv[]) {
    int thread_num;
    long rows, cols;                         /* dimensions of the pixmap */
    unsigned char image[MAXROWS][MAXCOLS];  /* 2D array to hold the image */
    unsigned char out[MAXROWS][MAXCOLS];

    int readOK, writeOK; // Error checking flags

    if (argc != 4) {
        printf("Usage: %s <input image path> <output file name> <num threads>\n", argv[0]);
        exit(1);
    }

    thread_num = atoi(argv[3]);
    readOK = pgmRead(argv[1], &rows, &cols, image);

    if (!readOK) {
        printf("Error reading file: %s!\n", argv[1]);
        exit(1);
    }

    int thetaBins = 20;
    int radiiBins = 20;
    edgeGradient(image, rows, cols, out, thread_num);

    unsigned char numLines = 5;
    Line *lines = lineDetect(out, rows, cols, thetaBins, radiiBins, thread_num, 150, numLines);

    int i;
    for (i = 0; i < numLines; i++) {
        printLine(lines[i]);

        int row, col;
        for (row = 0; row < rows; row++) {
            for (col = 0; col < cols; col++) {
                int radius = lines[i].radius;
                int theta = lines[i].theta;

                if (radius == (int) (col * cos(theta) + row * sin(theta))) {
                    image[row][col] = 255;
                }
            }
        }
    }

    free(lines);

    writeOK = pgmWrite(argv[2], rows, cols, image, NULL);

    if (writeOK) {
        printf("Successfully processed image!\n");
    }

}
