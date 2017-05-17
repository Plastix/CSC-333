/* Header file for PGM.c by Richard Zanibbi May 1998 for CISC 859 at
 * Queen's University. */


//NOTE - JR changed this from 2k to 1k to stay within stack sizes
#define MAXROWS 1200
#define MAXCOLS 1200
#define MAXLENGTH 256
#define MAXVALUE 255

int pgmRead (char *fileName,long *rows,long *cols,
  unsigned char image[][MAXCOLS]);

int pgmWrite(char* filename, long rows,long cols,
  unsigned char image[][MAXCOLS],char* comment_string);
