//to compile: mpicc -o bdayhash ./birthdayHash.c -lssl -lcrypto
//Note: remind John to start using Makefiles
#include <openssl/sha.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "Project2Utils.h"

#define RANDOM_STRING_LEN 10

//return 1 if collision is found, 0 otherwise
int collisionFound(char *collection, int sizeOfCollection, int *index1, int *index2) {
    int i, j;
    //braces in this syle for Akshay
    //although it makes me uncomfortable
    for (i = 0; i < sizeOfCollection - 1; i++) {
        for (j = i + 1; j < sizeOfCollection; j++) {
            if (collection[i] == collection[j]) {
                *index1 = i;
                *index2 = j;
                return 1;
            }
        }

    }
    return 0;
}

int collisionFound32(int *collection, int sizeOfCollection, int *index1, int *index2) {
    int i, j;
    //braces in this syle for Akshay
    //although it makes me uncomfortable
    for (i = 0; i < sizeOfCollection - 1; i++) {
        for (j = i + 1; j < sizeOfCollection; j++) {
            if (collection[i] == collection[j]) {
                *index1 = i;
                *index2 = j;
                return 1;
            }
        }

    }
    return 0;
}


//fill the collection with hashes of (random) strings
void makeHashCollection(char *collection, int sizeOfCollection) {
    //note to self:
    //better to make this a helper function
    int i;
    char buf1[10];
    unsigned char result1[SHA256_DIGEST_LENGTH];
    for (i = 0; i < sizeOfCollection; i++) {
        MakeRandomString(RANDOM_STRING_LEN, buf1);
        makeSHAHash(buf1, result1);
        collection[i] = result1[SHA256_DIGEST_LENGTH - 1];
    }


}

void makeHash32Collection(int *collection, int sizeOfCollection) {
    //note to self:
    //better to make this a helper function
    int i;
    char buf1[10];
    unsigned char result1[SHA256_DIGEST_LENGTH];
    for (i = 0; i < sizeOfCollection; i++) {
        MakeRandomString(RANDOM_STRING_LEN, buf1);
        makeSHAHash(buf1, result1);
        //printSHA256Hash(result1);
        //through abuse of types, I can cast the last two bytes (8 hex chars)
        //of a char array as an int!
        collection[i] = byteArrayToInt(&(result1[SHA256_DIGEST_LENGTH - 4]));
    }

}

int main() {
    srand(time(NULL));

//a birthday party with 1000 guests!
//we should have a 50% chance with a 32-bit hash
    int N = 77000;

    printf("size of int is %lu\n", sizeof(int));
    char *collectionOfHashes = malloc(N * sizeof(char));
    int *collectionOf32Hashes = malloc(N * sizeof(int));


    int found = 0;

    int collisionIndex1 = 0;
    int collisionIndex2 = 0;

    int tries = 0;

    while (1) {

        tries++;

        makeHashCollection(collectionOfHashes, N);

        if (collisionFound(collectionOfHashes, N, &collisionIndex1, &collisionIndex2)) {
            found = 1;
            printf("found a collision after %d tries!\n", tries);

            printf("0x");
            printf("%02x", (unsigned char) collectionOfHashes[collisionIndex1]);
            putchar('\n');

            /* Print the digest as one long hex value */
            printf("0x");
            printf("%02x", (unsigned char) collectionOfHashes[collisionIndex2]);
            putchar('\n');

            //ssh, don't tell intro students I did this.
            break;
        }
    }


    tries = 0;
    while (1) {

        printf("starting to make 32-hash of size %d\n", N);

        makeHash32Collection(collectionOf32Hashes, N);

        printf("done making 32-hash of size %d\n", N);

        if (collisionFound32(collectionOf32Hashes, N, &collisionIndex1, &collisionIndex2)) {
            found = 1;
            printf("found a 32-bit collision after %d tries!\n", tries);

            printf("0x");
            printf("%u", (unsigned) collectionOf32Hashes[collisionIndex1]);
            putchar('\n');

            /* Print the digest as one long hex value */
            printf("0x");
            printf("%u", (unsigned) collectionOf32Hashes[collisionIndex2]);
            putchar('\n');

            //ssh, don't tell intro students I did this.
            break;
        } else {
            printf("no luck on try %d\n", tries);
            tries++;
        }
    }

}
