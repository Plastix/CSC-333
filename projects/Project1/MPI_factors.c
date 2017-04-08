#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    int comm_sz;               /* Number of processes    */
    int my_rank;               /* My process rank        */
    //Don't put any MPI stuff before this line

    MPI_Init(&argc, &argv);  //sets up the MPI. Always this line!

    /* Get the number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* Get my rank among all the processes */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    // do some stuff

    if (argc != 2) {
        if (my_rank == 0) {
            printf("usage: %s <number>", argv[0]);
        }
    } else {

        // Read number from stdin
        unsigned long long bignumber = strtoull(argv[1], NULL, 10);
        long i;

        int my_factors = 0;

        for (i = my_rank + 1; i <= bignumber; i += comm_sz) {
            if ((bignumber % i) == 0) {
                my_factors++;
            }
        }

        if (my_rank != 0) {
            MPI_Send(&my_factors, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        } else {
            int q;
            int global_factors = my_factors;
            int other_factors;

            for (q = 1; q < comm_sz; q++) {
                MPI_Recv(&other_factors, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                global_factors += other_factors;
            }

            printf("Found a total of %d factors in %llu!\n", global_factors, bignumber);
        }

    }

    MPI_Finalize();
    //please no MPI stuff after this line

    return 0;
}
