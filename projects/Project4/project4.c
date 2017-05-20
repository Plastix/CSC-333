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

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

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

int *lineDetect(unsigned char inImg[MAXROWS][MAXCOLS], int rows, int cols,
                int thetaBins, int radiiBins, int thread_num,
                unsigned char threshold) {


    int *counts = calloc((size_t) (thetaBins * radiiBins), sizeof(int));

    int maxR = max(rows, cols);
    int radiiBinWidth = maxR / radiiBins;
    int thetaBinWidth = 180 / thetaBins;

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
                    int thetaBin = theta / thetaBinWidth;

                    counts[thetaBins * radiiBin + thetaBin]++;
                }
            }
        }
    }

    return counts;
}

void main(int argc, char *argv[]) {
    int thread_num;
    long rows, cols;                         /* dimensions of the pixmap */
    unsigned char image[MAXROWS][MAXCOLS];  /* 2D array to hold the image */
    unsigned char out[MAXROWS][MAXCOLS];

    int readOK, loopCondition, request;     /* two flags and user's request */
    int writeOK;                           /* flag for succesful write */


    if (argc != 4) {
        printf("Usage: %s <input image path> <output file name> <num threads>\n", argv[0]);
        exit(1);
    }

    thread_num = atoi(argv[3]);

    printf("input filename:\n"); /*  and manipulate it. */
    readOK = pgmRead(argv[1], &rows, &cols, image);

    if (!readOK) {
        printf("Error reading file: %s!\n", argv[1]);
        exit(1);
    }

    int thetaBins = 20;
    int radiiBins = 20;
    edgeGradient(image, rows, cols, out, thread_num);
    int *counts = lineDetect(out, rows, cols, thetaBins, radiiBins, thread_num, 200);

    for (int i = 0; i < radiiBins; i++) {
        for (int j = 0; j < thetaBins; j++) {
            printf("%i ", counts[i * thetaBins + j]);
        }
        printf("\n");
    }


    free(counts);

    writeOK = pgmWrite(argv[2], rows, cols, out, NULL);

    if (writeOK) {
        printf("Successfully processed image!\n");
    }

}
