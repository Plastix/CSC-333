/* demopgm.c  by Richard Zanibbi (May 1998) for
 * CISC 859 at Queen's University.
 *
 * This program produces a menu that the user may
 * select from to open PGM (P2 or P5) files and perform
 * simple image processing on them, or view a couple of 
 * simple synthesis techniques.  
 *
 * NOTE:  all output is stored (repeatedly if necessary) to
 *   the file demo.pgm.  The program calls xv to display demo.pgm
 *   after each request.
 */

#include "PGM.h"
#include "iroutines.h"
#include <stdio.h>
#include <stdlib.h>

void main (){
    long rows,cols;                         /* dimensions of the pixmap */
    unsigned char image[MAXROWS][MAXCOLS];  /* 2D array to hold the image */
    char userInput[60];                     /* input from user */
    int readOK,loopCondition,request;       /* two flags and user's request */
    int writeOK;                           /* flag for succesful write */
    loopCondition = 1;                     /* set loop condition to "true" */
    
    while (loopCondition) {
        /* print MENU, get user's input */
        printf ("\nMENU\n****\n1. Invert Pixmap\n2. Flip Horizontally\n");
        printf ("3. Flip Vertically\n4. Synthesis - Bars\n");
        printf ("5. Synthesis - Random\n");
        printf ("6. Exit\n\nEnter your request:\n>:");
        scanf ("%s",userInput);
        sscanf (userInput,"%d",&request);

	/* carry out user's request */
	switch (request) {
             case 6: loopCondition = readOK = 0; /* exit */
	          break;
             case 5: rows = MAXROWS;             /* 4 + 5 - synth routines */
                  cols = MAXCOLS;
                  synthRand (rows,cols,image);
		  readOK = 1;
                  break;            
             case 4: rows = MAXROWS;
                  cols = MAXCOLS;
                  synthBars (rows,cols,image);
		  readOK = 1;
                  break;
	     case 3: case 2: case 1:            /* 3,2,1 - load a file */ 
                  printf ("input filename:\n"); /*  and manipulate it. */
                  scanf ("%s",userInput);
                  readOK = pgmRead (userInput,&rows,&cols,image);
		  if (readOK) {
		       switch (request) {
			    /* use appropriate image processing routine */
	                    case 1: invert (rows,cols,image);
	                         break;
	                    case 2: flipHorizontal (rows,cols,image);
	                         break;
	                    case 3: flipVertical (rows,cols,image);
	                         break;
		       }
		  }
                  break;
        }	
   
	/* if file was read in properly (or synthesized) display the image */
        if (readOK) {            	    
             writeOK = pgmWrite ("demo.pgm",rows,cols,image,"test");
	     if (writeOK) {
	         system ("xv demo.pgm &"); /* system call */
	     }
        }	
    }
}
