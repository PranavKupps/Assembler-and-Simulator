#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MEMORYSIZE 65536
#define NUMREGS 8 /*total number of machine registers [0,7]*/

#define MAXLINELENGTH 1000


int main(int argc, char **argv) {
    
    int answer = 0;
    int mcand = 32767;
    int mplier = 32767;
    int bit_loc = 1;


    while(bit_loc != 32768) {
        int bit = ~(~bit_loc | ~mplier);
        if(bit == bit_loc) {
            answer += mcand;
        }

        bit_loc += bit_loc;
        mcand += mcand;
    }
    
    /*for(int i = 0; i < 15; ++i) {
        //state.reg[arg2] = ~(state.reg[arg0] | state.reg[arg1]); nor
        int bit = ~(~bit_loc | ~mplier);
        if(bit == bit_loc) {
            answer += mcand;
        }
        
        bit_loc += bit_loc;
        mcand += mcand;
    }*/

    printf("answer: %d", answer);

    return(0);
}
