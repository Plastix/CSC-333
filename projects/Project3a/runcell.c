#include <stdio.h>
#include "CellAut.h"

/* sat.c
 author: John Rieffel
 adapted from Quinn's Circuit Satisfiability probelm
*/


int main(int argc, char *argv[]) {

    int WORLDSIZE = 128;

    int *mycellworld = MakeCellWorld(WORLDSIZE);
    InitCellWorld(mycellworld, WORLDSIZE);

    if (argc > 1) {
        if (argc > 2) {
            WORLDSIZE = atoi(argv[2]);
        }
        RunCellWorld(mycellworld, WORLDSIZE, atoi(argv[1]));
    } else {
        printf("usage: runcell <rulenumber>\n");
        printf("usage: runcell <rulenumber> <worldsize>\n");
    }

    //command line arguments are passed in as an array of strings!
    //where argc says how many arguments there are
    //(note the executable itself is the first argument, so argc is always 1)


}
