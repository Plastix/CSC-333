#include <stdio.h>
#include <memory.h>
#include "CellAut.h"
#include "mpi.h"

/* sat.c
 author: John Rieffel
 adapted from Quinn's Circuit Satisfiability probelm
*/


#define ROOT_NODE 0

// We need to get values from our neighbors, and send values to our neighbors
// how to do it without deadlock?
// MPI_Send is non-blocking, except when it isn't
//
// When this function returns,
// leftval should contain the value sent by your left neighbor
// and rightval should contain the value sent by yrou right neighbor
void DistributeLeftAndRightVals(int myID, int numprocs, int *mylocalcells, int localsize, int *leftval, int *rightval) {
    int rightID = (myID + 1) % numprocs;  //neighbor to your right
    int leftID = (myID - 1 + numprocs) % numprocs; //neighbor to your left

    int myfirst = mylocalcells[0];
    int mylast = mylocalcells[localsize - 3];

    *leftval = 0;
    *rightval = 0;

    MPI_Request lreq;
    MPI_Isend(&myfirst, 1, MPI_INT, leftID, 0, MPI_COMM_WORLD, &lreq);

    MPI_Request rreq;
    MPI_Isend(&mylast, 1, MPI_INT, rightID, 0, MPI_COMM_WORLD, &rreq);

    MPI_Request lrecv;
    MPI_Irecv(leftval, 1, MPI_INT, leftID, 0, MPI_COMM_WORLD, &lrecv);

    MPI_Request rrecv;
    MPI_Irecv(rightval, 1, MPI_INT, rightID, 0, MPI_COMM_WORLD, &rrecv);

    MPI_Request requests[] = {lreq, rreq, lrecv, rrecv};
    MPI_Waitall(4, requests, MPI_STATUSES_IGNORE);

    printf("I am node %d, I got %d from left node %d and %d from right node %d\n", myID, *leftval, leftID, *rightval,
           rightID);

}

void MPIRunCellWorld(int myID, int numprocs, int *localcells, int localsize, int iterations, int rule) {
    int leftval = 0;
    int rightval = 0;
    int *newcells = MakeCellWorld(localsize);

    int itercount = 0;
    for (itercount = 0; itercount < iterations; itercount++) {
        // printf("node %d before rules\n",myID);
        // printWorld(localcells,localsize,myID);
        DistributeLeftAndRightVals(myID, numprocs, localcells, localsize, &leftval, &rightval);

        //tricky. Assuming we've padded localworld with two extra indexes
        //We can put the right neigbhor's value at the second to last index
        // and the left neighbor's value at the last index
        //
        // i.e. if size of localworld L is actually five (5)
        // index: 0     1    2    3    4    5  6
        //       L[0] L[1] L[2] L[3] L[4]   R  L
        // and then if we treat the array as the same torus/ring, the math works
        // with the same function as the serial version.
        localcells[localsize - 2] = rightval;
        localcells[localsize - 1] = leftval;
        int locali = 0;
        for (locali = 0; locali < localsize; locali++) {
            ApplyRuleAtLoc(localcells, newcells, locali, localsize, rule);
        }
        // the world becomes the new world
        // and new world becomes the old world
        // (this way we only have to allocate the array once)
        //       int *oldcells = localcells;
        //localcells = newcells;
        //newcells = oldcells;
        memcpy((void *) localcells, (void *) newcells, localsize * sizeof(int));
        //   printf("node %d after rules\n",myID);
        // printWorld(localcells,localsize,myID);

    }

}

int main(int argc, char *argv[]) {
    int i;
    int id;               /* Process rank */
    int p;                /* Number of processes */

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int WORLDSIZE = 128;

    int *localcells; //local array
    int localsize = (WORLDSIZE / p) + 2;  //size of local array
    //plus two extra indexes, one for each neighbor

    printf("node :%d, worldsize: %d, localsize: %d\n", id, WORLDSIZE, localsize);

    if (WORLDSIZE % p != 0) {
        printf("Worldsize must be divisible by processors!");
        MPI_Finalize();
        exit(1);
    }

    if ((localcells = malloc(localsize * sizeof(int))) == NULL) {
        printf("Memory allocation error!");
        MPI_Finalize();
        exit(1);
    }
    //everyone creates their own copy of the world
    int *mycellworld;

    if (id == 0) {
        mycellworld = MakeCellWorld(WORLDSIZE);
        InitCellWorld(mycellworld, WORLDSIZE);
    }

    // scatter global copy to rest of nodes' local world
    MPI_Scatter(mycellworld, localsize - 2, MPI_INT, localcells, localsize - 2, MPI_INT, ROOT_NODE, MPI_COMM_WORLD);
    //now everyone has their own slice of the world!

    printf("I am node %d, my local world is now:\n", id);
    printWorld(localcells, localsize, id);


    int iterstep = 1;
    int curiters = 0;
    int maxiters = 100;
    int rule = 30;

    for (curiters = 0; curiters < maxiters; curiters++) {
        MPIRunCellWorld(id, p, localcells, localsize, iterstep, rule);
        //     printf("I am node %d, my local world is now:\n",id);
        //printWorld(localcells,localsize,id);

        //command line arguments are passed in as an array of strings!
        //where argc says how many arguments there are
        //(note the executable itself is the first argument, so argc is always 1)


        //now we need to use Gather to get the distributed data back to node 0
        //so we can print it out
        MPI_Gather(localcells, localsize - 2, MPI_INT, mycellworld, localsize - 2, MPI_INT, ROOT_NODE, MPI_COMM_WORLD);

        if (id == 0) {
            printf("results of gather on node 0\n");
            printWorld(mycellworld, WORLDSIZE, id);
        }
    }

    MPI_Finalize();
}


