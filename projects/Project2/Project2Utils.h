//utility code for Project 2
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void MakeRandomString(int string_length, char *thestring) {
    for (int i = 0; i < string_length; ++i) {
        thestring[i] = '0' + rand() % 94; // starting on '0', ending on 'z'
    }
}

void printSHA256Hash(unsigned char *inhash) {
    size_t n;
    for (n = 0; n < SHA256_DIGEST_LENGTH; n++)
        printf("%02x", inhash[n]);
    putchar('\n');
}


//magic
void makeSHAHash(char *buf, unsigned char *results) {
    SHA256_CTX ctx;
    size_t n;

    n = strlen(buf);
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, buf, n);
    SHA256_Final(results, &ctx);

}

//given a pointer byte array (like from a SHA-256 hash) of length FOUR
//return the last 4 bytes as a 4-byte integer
// so the byte array [12,34,45,68]
// becomes the int-32 0x12345678
// which happens to be the (unsigned) int 0d305419896
// to test, put this hex value
unsigned int byteArrayToInt(unsigned char *inByteArray) {
    unsigned char byte3 = *inByteArray;
    unsigned char byte2 = *(inByteArray + 1);
    unsigned char byte1 = *(inByteArray + 2);
    unsigned char byte0 = *(inByteArray + 3);

    //printf("%02x %02x %02x %02x\n",byte3,byte2,byte1,byte0);
    // the MSB left shifted by 24 BITWISE-OR
    // the next byle left shifted by 16 BITWISE-OR
    // the next byte left shifted by 8 BITWISE-OR
    // the LSB not shifted at all
    return byte3 << 24 | byte2 << 16 | byte1 << 8 | byte0;
}

//the same, but for 8 bytes
//corresponding to a long
unsigned long byteArrayToLong(unsigned char *inByteArray) {
    unsigned int MostSignificantInt = byteArrayToInt(inByteArray);
    unsigned int LeastSignificantInt = byteArrayToInt(inByteArray + 4);
    unsigned long result = (unsigned long) MostSignificantInt << 32 | LeastSignificantInt;
    return result;
}
