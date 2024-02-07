/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure to NOT modify printState or any of the associated functions
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//DO NOT CHANGE THE FOLLOWING DEFINITIONS 

// Machine Definitions
#define MEMORYSIZE 65536 /* maximum number of words in memory (maximum number of lines in a given file)*/
#define NUMREGS 8 /*total number of machine registers [0,7]*/

// File Definitions
#define MAXLINELENGTH 1000 /* MAXLINELENGTH is the max number of characters we read */

typedef struct 
stateStruct {
    int pc;
    int mem[MEMORYSIZE];
    int64_t reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);

static inline int convertNum(int32_t);

int main(int argc, char **argv) {
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s , please ensure you are providing the correct path", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; ++state.numMemory) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address  %d\n. Please ensure you are providing a machine code file.", state.numMemory);
            perror("sscanf");
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    //Your code starts here!

    for(int i = 0; i < 8; ++i) {
        state.reg[i] = 0;
    }

    bool done = false;
    state.pc = 0;
    int total_instructions = 0;

    while(!done) {
        printState(&state);

        //printf("current memory: %d\n", state.mem[state.pc]);
        
        int offset = convertNum(state.mem[state.pc] & 65535);
        int opcode = (state.mem[state.pc] & 29360128) >> 22;
        int arg0 =   (state.mem[state.pc] & 3670016)  >> 19;
        int arg1 =   (state.mem[state.pc] & 458752) >> 16;
        int arg2 =   state.mem[state.pc] & 7;


        /*if(offset == -27) {
            printf("found -27\n");
            printf("size: %d\n", state.numMemory);
            break;
        }*/

        if(offset > state.numMemory || offset < ((state.numMemory) * -1)) {
            printf("offset is not in memory range");
            exit(1);
        }

        if(opcode == 0) { //add
            printf("add\n");
            state.reg[arg2] = state.reg[arg0] + state.reg[arg1];
            state.pc++;
        } else if(opcode == 1) { //nor
            //printf("now\n");
            state.reg[arg2] = ~(state.reg[arg0] | state.reg[arg1]);

            state.pc++;
        } else if(opcode == 2) { //lw
            printf("load word\n");
            state.reg[arg1] = state.mem[state.reg[arg0] + offset];
            state.pc++;
        } else if(opcode == 3) { //sw
            printf("store word\n");
            state.mem[state.reg[arg0] + offset] = state.reg[arg1];
            state.pc++;
        } else if(opcode == 4) { //beq
            //printf("beq state.pc == %d\n", state.pc);
            if(state.reg[arg0] == state.reg[arg1]) {
                state.pc += offset + 1;
            } else {
                state.pc++;
            }
            //printf("beq state.pc == %d\n", state.pc);
        } else if(opcode == 5) { //jalr
            //printf("jalr\n");
            state.reg[arg1] = state.pc + 1;
            //printf("jalr store: %d\n", state.reg[arg1]);
            state.pc = state.reg[arg0];
            //printf("pc: %d\nregisterA: %d\n", state.pc, state.reg[arg0]);
        } else if(opcode == 6) { //halt
            //printf("halt\n");
            done = true;
            state.pc++;
        } else if(opcode == 7) { //noop
            //printf("noop\n");
            state.pc++;
        } else {
            printf("there is not identidiable opcode");
            exit(1);
        }

        //state.pc++;

        if(state.pc == state.numMemory) {
            done = true;
        } else if(state.pc > state.numMemory) {
            printf("pc is greater than memory");
            exit(1);
        }
        total_instructions++;
        printf("instruction #%d \n", total_instructions);

    }

    //state.reg[1] += 32767;
    printf("machine halted\ntotal of %d instructions executed\nfinal state of machine:\n", total_instructions);

    //printf("instruction #%d \n", total_instructions);
    printState(&state);

    //Your code ends here! 

    return(0);
}

/*
* DO NOT MODIFY ANY OF THE CODE BELOW. 
*/

void 
printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) 
              printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) 
              printf("\t\treg[ %d ] %lld\n", i, statePtr->reg[i]);
    printf("end state\n");
}

// convert a 16-bit number into a 32-bit Linux integer
static inline int convertNum(int num) 
{
    return num - ( (num & (1<<15)) ? 1<<16 : 0 );
}

/*
* Write any helper functions that you wish down here. 
*/
