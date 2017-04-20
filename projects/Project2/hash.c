#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void MakeRandomString(int string_length, char *thestring) {
    for (int i = 0; i < string_length; ++i) {
        thestring[i] = '0' + rand() % 52; // starting on '0', ending on '}'
    }
}


//to compile: mpicc -o hash ./hash.c -lssl -lcrypto
void makeSHAHash(char *buf, unsigned char *results) {
    SHA256_CTX ctx;
    size_t n;

    n = strlen(buf);
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, buf, n);
    SHA256_Final(results, &ctx);

}

int main() {
    srand(time(NULL));
    char buf1[10];
    char buf2[10];
    size_t n;

    unsigned char result1[SHA256_DIGEST_LENGTH];
    unsigned char result2[SHA256_DIGEST_LENGTH];

    printf("digest length is: %d\n", SHA256_DIGEST_LENGTH);

    int tries = 0;

    while (1) {

        MakeRandomString(10, buf1);
        MakeRandomString(10, buf2);


        makeSHAHash(buf1, result1);
        makeSHAHash(buf2, result2);

        /* Print the digest as one long hex value */
        printf("0x");
        for (n = 0; n < SHA256_DIGEST_LENGTH; n++)
            printf("%02x", result1[n]);
        putchar('\n');

        /* Print the digest as one long hex value */
        printf("0x");
        for (n = 0; n < SHA256_DIGEST_LENGTH; n++)
            printf("%02x", result2[n]);
        putchar('\n');

        tries++;

        if (result1[SHA256_DIGEST_LENGTH - 1] == result2[SHA256_DIGEST_LENGTH - 1])
            break;
    }
    printf("that took %d tries to find a collision\n", tries);
}
