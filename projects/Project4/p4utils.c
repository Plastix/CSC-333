/*

project 4 utilities

*/
#include "p4utils.h"
void cannyX(unsigned char inImg[MAXROWS][MAXCOLS],long rows, long cols,unsigned char outimg[MAXROWS][MAXCOLS]){

  int cannyX[3][3] = {{1, 0, -1},{2, 0, -2},{1, 0,-1}};
  int row,col;
  for (row = 0; row < rows; row++){
    for (col = 0; col < cols; col++){

      long acc = 0;
        //instead of wrapping around a toroid we just repeat first/last coloum and row
        int krow, kcol;

        for  (krow = 0; krow < 3; krow++){
          for (kcol = 0; kcol < 3; kcol++){
              //get the kernel value
              int kval = cannyX[krow][kcol];
              //printf("kval is: %d",kval);

              int krowoffset = krow -1;
              int kcoloffset = kcol -1;
              int currow = row + krowoffset;
              int curcol = col + kcoloffset;

              currow = currow < 0? 0 : currow;
              curcol = curcol < 0? 0 :curcol;
              currow = currow > rows -1 ? rows -1 :currow;
              curcol = curcol > cols - 1? cols - 1:curcol;
              acc += inImg[currow][curcol]*kval;
          }
          }
          outimg[row][col] = acc;
      }
  }

}

void convolve(unsigned char inImg[MAXROWS][MAXCOLS],long rows, long cols,unsigned char outimg[MAXROWS][MAXCOLS], int kernel[3][3])
{
  int row,col;
  for (row = 0; row < rows; row++){
    for (col = 0; col < cols; col++){

      long acc = 0;
        //instead of wrapping around a toroid we just repeat first/last coloum and row
        int krow, kcol;

        for  (krow = 0; krow < 3; krow++){
          for (kcol = 0; kcol < 3; kcol++){
              //get the kernel value
              int kval = kernel[krow][kcol];
              //printf("kval is: %d",kval);

              int krowoffset = krow -1;
              int kcoloffset = kcol -1;
              int currow = row + krowoffset;
              int curcol = col + kcoloffset;

              currow = currow < 0? 0 : currow;
              curcol = curcol < 0? 0 :curcol;
              currow = currow > rows -1 ? rows -1 :currow;
              curcol = curcol > cols - 1? cols - 1:curcol;
              acc += inImg[currow][curcol]*kval;
          }
          }
          outimg[row][col] = acc;
      }
  }

  }
  
