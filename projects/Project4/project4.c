/*  project4.c
by John Rieffel, Union College

based initially on demopgm.c  by Richard Zanibbi (May 1998) for
 * CISC 859 at Queen's University.
 *

 Description: shows a simple sobel-Filter convolution of a pgm image
 */

#include "PGM.h"
#include "iroutines.h"
#include "p4utils.h"
#include <stdio.h>
#include <stdlib.h>

void main (){
    long rows,cols;                         /* dimensions of the pixmap */
    unsigned char image[MAXROWS][MAXCOLS];  /* 2D array to hold the image */

    unsigned char xsobel[MAXROWS][MAXCOLS];
    unsigned char ysobel[MAXROWS][MAXCOLS];
    char userInput[60];                     /* input from user */
    int readOK,loopCondition,request;       /* two flags and user's request */
    int writeOK;                           /* flag for succesful write */
    loopCondition = 1;                     /* set loop condition to "true" */

    printf ("input filename:\n"); /*  and manipulate it. */
    scanf ("%s",userInput);
    readOK = pgmRead (userInput,&rows,&cols,image);

    // printf("there are %lu rows and %lu cols\n",rows,cols);
     int row, col;
    // for (row = 0; row < rows; row++){
    //   for (col = 0; col < cols; col++) {
    //       printf("%u ",image[row][col]);
    //   }
    //   printf("\n");
    // }


    int sobelXkernel[3][3] = {{1, 0, -1},{2, 0, -2},{1, 0,-1}};
    int sobelYkernel[3][3] = {{-1, -2, -1},{0, 0, 0},{1, 2, 1}};

    //fill the xsobel matrix with the convolution
    // of the X sobel Kernel
    convolve(image,rows,cols,xsobel,sobelXkernel);
    //fill the ysobel matrix with the convolution
    // of the y sobel Kernel
    convolve(image,rows,cols,ysobel,sobelYkernel);


    // for (row = 0; row < rows; row++){
    //   for (col = 0; col < cols; col++) {
    //       printf("%u ",ysobel[row][col]);
    //   }
    //   printf("\n");
    // }

   //write the two files
   pgmWrite("xsobel.pgm",rows,cols,xsobel,"test");
   pgmWrite("ysobel.pgm",rows,cols,ysobel,"test");

}
