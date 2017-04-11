#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    int comm_sz;               /* Number of processes    */
    int my_rank;               /* My process rank        */
    //Don't put any MPI stuff before this line

    // Timing variables
    double local_start, local_finish, local_elapsed, elapsed;

    MPI_Init(&argc, &argv);  //sets up the MPI. Always this line!

    /* Get the number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* Get my rank among all the processes */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    // do some stuff

    if (argc != 2) {
        if (my_rank == 0) {
            printf("usage: %s <number>\n", argv[0]);
        }
    } else {

        // Read number from stdin
        unsigned long long bignumber = strtoull(argv[1], NULL, 10);

        MPI_Barrier(MPI_COMM_WORLD); // Sync up processes
        local_start = MPI_Wtime();

        long i;
        int my_factors = 0;
        int global_factors;

        for (i = my_rank + 1; i <= bignumber; i += comm_sz) {
            if ((bignumber % i) == 0) {
                my_factors++;
            }
        }

        MPI_Reduce(&my_factors, &global_factors, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        local_finish = MPI_Wtime();
        local_elapsed = local_finish - local_start;
        
        MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

        if (my_rank == 0) {
            printf("Found a total of %d factors in %llu!\n", global_factors, bignumber);
            printf("Elapsed time = %e seconds\n", elapsed);
        }

    }

    MPI_Finalize();
    //please no MPI stuff after this line

    return 0;
}
