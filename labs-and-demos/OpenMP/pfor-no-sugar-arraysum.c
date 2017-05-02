#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    int PiVals[10] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 4};
    long nthreads = 1;

    if (argc > 1)
        nthreads = strtol(argv[1], NULL, 10);

    int globalsum = 0;

#pragma omp parallel num_threads(nthreads)
    {
        int myid = omp_get_thread_num();
        int threadcount = omp_get_num_threads(); //like, super-high threadcount sheets?
        int localsum = 0;
        for (int i = myid; i < 10; i += threadcount) {
            //do something here to accumulate a local sum
            localsum += PiVals[i];
        }
#pragma omp critical
        globalsum += localsum;
        //do something here to (atomically) aggregate local sums into a global sum
    } //end of pfor pragma

    printf("global sum is %d\n", globalsum);
}
