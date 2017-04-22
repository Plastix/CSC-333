#include <mpi.h>
#include "Project2Utils.h"
#include <stdarg.h>

#define RANDOM_STRING_LEN 10
#define ROOT_NODE 0

// Only print to stdout on root node
void safeprintf(const int my_rank, const char *format, ...) {
    if (my_rank == ROOT_NODE) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

int main(int argc, char *argv[]) {

    int comm_sz;               /* Number of processes    */
    int my_rank;               /* My process rank        */

    //Don't put any MPI stuff before this line
    MPI_Init(&argc, &argv);  //sets up the MPI. Always this line!

    /* Get the number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* Get my rank among all the processes */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (argc != 3) {
        safeprintf(my_rank, "usage: %s <num hashes> <bit window>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    double local_start, local_finish, local_elapsed, elapsed;

    // Set up pseudo random num generator
    time_t seed = time(NULL);
    srand((unsigned int) seed);

    MPI_Barrier(MPI_COMM_WORLD);
    local_start = MPI_Wtime();

    unsigned int n = (unsigned int) strtoul(argv[1], NULL, 10);
    int bits = atoi(argv[2]);

    if (bits < 32 || bits > 63) {
        safeprintf(my_rank, "Bit window must be be in range 32-63. %i was given.\n", bits);
        exit(EXIT_FAILURE);
    }

    int collision = 0;
    int tries = 1;
    unsigned long buffer[n];

    while (1) {

        safeprintf(my_rank, "Generating %i hashes on root node...\n", n);
        if (my_rank == ROOT_NODE) {
            char buf1[10];
            unsigned char result1[32];
            for (int i = 0; i < n; i++) {
                MakeRandomString(RANDOM_STRING_LEN, buf1);
                makeSHAHash(buf1, result1);
                buffer[i] = byteArrayToLong(&(result1[SHA256_DIGEST_LENGTH - 8]));
            }
        }

        safeprintf(my_rank, "Broadcasting hashes to all nodes...\n");
        MPI_Bcast(buffer, n, MPI_UNSIGNED_LONG, ROOT_NODE, MPI_COMM_WORLD);

        safeprintf(my_rank, "Checking for collisions...\n");
        unsigned int i, j;
        for (i = 0; i < n - 1 && !collision; i++) {
            for (j = my_rank + i + 1; j < n && !collision; j += comm_sz) {
                unsigned long h1 = (buffer[i] & ((1ul << bits) - 1));
                unsigned long h2 = (buffer[j] & ((1ul << bits) - 1));

                if (h1 == h2) {
                    collision = 1;
                }
            }

            MPI_Allreduce(MPI_IN_PLACE, &collision, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
        }

        if (collision) {
            break;
        }

        safeprintf(my_rank, "Found no collisions. Trying again...\n");
        tries++;
    }

    local_finish = MPI_Wtime();
    local_elapsed = local_finish - local_start;
    MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, ROOT_NODE, MPI_COMM_WORLD);

    safeprintf(my_rank, "\nCollision found in %i tries (%i hashes generated)\n", tries, n * tries);
    safeprintf(my_rank, "Time: %e sec\n", elapsed);
    safeprintf(my_rank, "Random seed: %i", seed);

    MPI_Finalize();
    return 0;
}


