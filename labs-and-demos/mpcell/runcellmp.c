#include <stdio.h>
#include "CellAut.h"
#include <omp.h>

/* runcellmp.c
 author: John Rieffel
 description: should be no different than runcell.c
 but will allow for OpenMP invocations of RunCellWorld.
*/


int main(int argc, char *argv[]) {

    int WORLDSIZE = 128;
    int iters = -1;
    int threads = 1;

    if (argc > 1) {
        if (argc > 2) {
            WORLDSIZE = atoi(argv[2]);
            if (argc > 3) {
                iters = atoi(argv[3]);
                if (argc > 4)
                    threads = atoi(argv[4]);
            }


        }
        int *mycellworld = MakeCellWorld(WORLDSIZE);
        InitCellWorld(mycellworld, WORLDSIZE);
        double start = omp_get_wtime();
        RunCellWorld(mycellworld, WORLDSIZE, atoi(argv[1]), iters, threads);
        double end = omp_get_wtime();
        double elapsed = end - start;
        printf("time: %f\n", elapsed);
    } else {
        printf("usage: runcell <rulenumber>\n");
        printf("usage: runcell <rulenumber> <worldsize> <iterations> <threads>\n");
    }

    //command line arguments are passed in as an array of strings!
    //where argc says how many arguments there are
    //(note the executable itself is the first argument, so argc is always 1)


}
