#include <stdio.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>

// Aidan Pieper

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("usage: %s <number>", argv[0]);
    } else {
        //printf ("sizeof unsigned long: %lu",sizeof(unsigned long));
        //printf ("sizeof unsigned long long: %lu",sizeof(unsigned long long));
        unsigned long long bignumber = strtoull(argv[1], NULL, 10);
        unsigned long i;

        clock_t startTime = clock();
        for (i = 3; i < sqrt(bignumber); i += 2) {
            if ((bignumber % i) == 0) {
                printf("I found %lu\n", i);
                break;
            }
        }

        clock_t endTime = clock();
        double elapsed = (double) (endTime - startTime) * 1000.0 / CLOCKS_PER_SEC;
        printf("that took %f ms\n", elapsed);
    }
}
