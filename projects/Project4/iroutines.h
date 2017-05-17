/* Header file for iroutines.c by Richard Zanibbi May 1998
 * for CISC 859 at Queen's University */

void invert (long rows,long cols,unsigned char image[MAXROWS][MAXCOLS]);
void flipVertical (long rows,long cols,unsigned char image[MAXROWS][MAXCOLS]);
void flipHorizontal (long rows,long cols,
		     unsigned char image[MAXROWS][MAXCOLS]);
void synthBars (long rows,long cols,unsigned char image[MAXROWS][MAXCOLS]);
void synthRand (long rows,long cols,unsigned char image[MAXROWS][MAXCOLS]);

