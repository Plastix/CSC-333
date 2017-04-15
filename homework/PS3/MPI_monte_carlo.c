#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <printf.h>

double randPos();

int main(int argc, char *argv[]) {

    int comm_sz;               /* Number of processes    */
    int my_rank;               /* My process rank        */

    MPI_Init(&argc, &argv);  //sets up the MPI. Always this line!

    /* Get the number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* Get my rank among all the processes */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    srand((unsigned int) time(NULL));   // should only be called once

    long long int number_of_tosses = 0;

    if (my_rank == 0) {
        printf("how many tosses do you want?\n");
        scanf("%lli", &number_of_tosses);
    }

    MPI_Bcast(&number_of_tosses, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    long long int global_number_in_circle = 0;
    long long int local_number_in_circle = 0;

    int toss;
    for (toss = my_rank; toss < number_of_tosses; toss += comm_sz) {
        double x = randPos();
        double y = randPos();
        double dist_square = x * x + y * y;
        if (dist_square <= 1) {
            local_number_in_circle++;
        }
    }

    MPI_Reduce(&local_number_in_circle, &global_number_in_circle, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        double pi = (4 * global_number_in_circle) / ((double) number_of_tosses);
        printf("Pi estimate: %lf", pi);
    }

    MPI_Finalize();
    return 0;
}

double randPos() {
    return (double) rand() / RAND_MAX * 2.0 - 1.0;
}
