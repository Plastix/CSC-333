#include "CellAut.h"

//allocate integer array of size size
int *MakeCellWorld(int size) {
    int *outworld;
    if ((outworld = malloc(size * sizeof(int))) == NULL) {
        printf("Memory allocation error!");
        return NULL;
    }
    return outworld;
}

//put a 1 in the middle
void InitCellWorld(int *inworld, int insize) {
    inworld[insize / 2] = 1;
}

//MPI-capable version of ApplyRule
//given a world, apply a rule at a particular location, where leftval and rightval come from neighbors (i.e. they are only used if loc == 0 or loc == size-1)
/*
void ApplyRuleAtLocWithNeighbors(int *oldworld, int *newworld, int loc,int worldsize, int rule, int leftval, int rightval)
{
    //with a neighborhood of 3, there are 8 possible cases we need to cover
  // 000 through 111, where the digits correspond to the left neighbor,
  // the current cell, and the right neighbor.
  // Rule 30, for instance, is
  // 111  110  101  100  011  010  001  000
  //  0    0    0    1    1    1    1    0  <- binary 30
  int ruleset[8]; //our rules
  int i;
  for (i = 0; i < 8; i++)
    ruleset[i] = EXTRACT_BIT(rule,i);


  //the neighborhood we're interested in
  int hood[3];

  hood[1] = oldworld[loc];
  //check boundary conditions
  hood[0] = ((loc - 1) >= 0) ? oldworld[loc-1] : leftval;
  hood[2] = ((loc + 1) < worldsize) ? oldworld[loc+1] : rightval;

  //find out which of the rules in the rulest to apply
  // i.e. if hood is 000, then apply ruleset[0]
  // and if hood is 010, apply ruleset[2]
  // better to use the same binary voodoo as extract_bit, neh?
  int ruleindex = hood[0] + 2*hood[1] + 4*hood[2];

  //  printf("hood of loc %d is [%d%d%d], applying ruleindex %d: %d%d%d->%d\n",loc, hood[2],hood[1],hood[0], ruleindex, hood[2],hood[1],hood[0], ruleset[ruleindex]);

  //change the new world
  newworld[loc] = ruleset[ruleindex];

}
*/
//see above MPI-capable function for more detail
void ApplyRuleAtLoc(int *oldworld, int *newworld, int loc, int worldsize, int rule) {
    //with a neighborhood of 3, there are 8 possible cases we need to cover
    // 000 through 111, where the digits correspond to the left neighbor,
    // the current cell, and the right neighbor.
    // Rule 30, for instance, is
    // 111  110  101  100  011  010  001  000
    //  0    0    0    1    1    1    1    0  <- binary 30
    int ruleset[8]; //our rules
    int i;
    for (i = 0; i < 8; i++)
        ruleset[i] = EXTRACT_BIT(rule, i);


    int hood[3];

    hood[1] = oldworld[loc];
    //check boundary conditions
    hood[0] = ((loc - 1) >= 0) ? oldworld[loc - 1] : oldworld[worldsize - 1];
    hood[2] = ((loc + 1) < worldsize) ? oldworld[loc + 1] : oldworld[0];

    // better to use the same binary voodoo as extract_bit, neh?
    int ruleindex = hood[0] + 2 * hood[1] + 4 * hood[2];

    //printf("hood is [%d%d%d], applying ruleindex %d: %d%d%d->%d\n", hood[2],hood[1],hood[0], ruleindex, hood[2],hood[1],hood[0], ruleset[ruleindex]);
    newworld[loc] = ruleset[ruleindex];

}

//print, taking into account node id
void printWorld(int *world, int worldsize, int myid) {
    int i;

    printf("(%d) :", myid);
    for (i = 0; i < worldsize; i++) {
        if (world[i] == 1)
            printf("#");
            //printf("%d",world[i]);
        else
            printf(" ");
    }
    printf("\n");
}

//serial version of run cell world
void RunCellWorld(int *world, int worldsize, int rule) {

    int *newworld = MakeCellWorld(worldsize);

    while (1) {
        int loc;
        printWorld(world, worldsize, -1);
        for (loc = 0; loc < worldsize; loc++)
            ApplyRuleAtLoc(world, newworld, loc, worldsize, rule);

        // the world becomes the new world
        // and new world becomes the old world
        // (this way we only have to allocate the array once)
        int *oldworld = world;
        world = newworld;
        newworld = oldworld;
    }
}
