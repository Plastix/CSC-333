#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

void randomizeMat(double *mat, int n, short *seed) {
    int row, col;

    for (row = 0; row < n; row++) {
        for (col = 0; col < n; col++) {
            mat[n * row + col] = erand48(seed);
        }
    }
}

void printMat(double *mat, int n) {
    int row, col;
    for (row = 0; row < n; row++) {
        for (col = 0; col < n; col++) {
            printf("%f ", mat[n * row + col]);
        }
        printf("\n");
    }
    printf("\n");

}

void sum(double *mat1, double *mat2, int n, int threads, double *outMat) {
    int row, col;
    for (row = 0; row < n; row++) {

#pragma omp parallel for num_threads(threads)
        for (col = 0; col < n; col++) {
            outMat[n * row + col] = mat1[n * row + col] + mat2[n * row + col];
        }
    }
}

void transpose(double *mat, int n, int threads, double *outMat) {
    int row, col;
    for (row = 0; row < n; row++) {

#pragma omp parallel for num_threads(threads)
        for (col = 0; col < n; col++) {
            outMat[n * row + col] = mat[col * n + row];
        }
    }
}

int main(int argc, char *argv[]) {

    int N = 0;

    // THIS WILL BE YOUR RANDOM SEED
    // YES IT IS NON-RANDOM - Read the lab!
    unsigned short xi[3] = {1, 9, 99};
    int threadCount;

    if (argc != 3) {
        printf("usage: matrices <Matrix Size> <Num Threads>\n ");
        exit(1);
    } else {
        N = atoi(argv[1]);
        threadCount = atoi(argv[2]);

//        printf("Rows: %d, Cols: %d\n", N, N);
    }

    double *MAT = calloc(N * N, sizeof(double));
    double *MAT2 = calloc(N * N, sizeof(double));
    double *sumsMAT = calloc(N * N, sizeof(double));
    double *transposeMat = calloc(N * N, sizeof(double));

    randomizeMat(MAT, N, xi);
    randomizeMat(MAT2, N, xi);

    double start = omp_get_wtime();
    sum(MAT, MAT2, N, threadCount, sumsMAT);
    double end = omp_get_wtime();
    double sum_elapsed = end - start;
    start = omp_get_wtime();
    transpose(MAT, N, threadCount, transposeMat);
    end = omp_get_wtime();
    double transpose_elapsed = end - start;
//
//    printf("MAT1\n");
//    printMat(MAT, N);
//    printf("MAT2\n");
//    printMat(MAT2, N);
//    printf("ADD\n");
//    printMat(sumsMAT, N);
//    printf("TRANSPOSE\n");
//    printMat(transposeMat, N);

    printf("Add Time: %e secs\n", sum_elapsed);
    printf("Transpose Time: %e secs\n", transpose_elapsed);

    free(MAT);
    free(MAT2);
    free(sumsMAT);
}




