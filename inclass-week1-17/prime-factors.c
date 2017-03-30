#include <stdio.h>
#include <time.h>
#include <math.h>
#include <limits.h>

int main(int argc, char *argv[])
{

  //printf ("sizeof unsigned long: %lu",sizeof(unsigned long));
  //printf ("sizeof unsigned long long: %lu",sizeof(unsigned long long));
  unsigned long long bignumber = 4185633689926419097;
  unsigned long i;

  clock_t startTime = clock();
  for (i = 3; i < ULLONG_MAX; i++)
  {
    if ((bignumber % i) == 0)
    {
      printf("I found %lu\n",i);
      break;
    }
  }
  clock_t endTime = clock();
  double elapsed =  (double)(endTime - startTime) * 1000.0 / CLOCKS_PER_SEC;
  printf("that took %f ms\n",elapsed);
}
