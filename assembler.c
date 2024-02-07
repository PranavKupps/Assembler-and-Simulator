/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

struct Symbolic {
    char label[7];
    int address;
};

int readAndParse(FILE *, char *, char *, char *, char *, char *);
static void checkForBlankLinesInCode(FILE *inFilePtr);
static inline int isNumber(char *);
int r_type(char *arg0, char *arg1, char *arg2, char* type, struct Symbolic addresses[MAXLINELENGTH], int labels, int address);
int i_type(char *arg0, char *arg1, char *arg2, char* type, struct Symbolic addresses[MAXLINELENGTH], int labels, int address);
int j_type(char* arg0, char* arg1);
int o_type(char* type);
int dot_fill(char* arg0, int address, int labels, struct Symbolic addresses[MAXLINELENGTH]);

int main(int argc, char **argv) {
    char *inFileString;
    char *outFileString;
    FILE *inFilePtr;
    FILE *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    int address = 0;
    int labels = 0;
    struct Symbolic instructions[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
        argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }

    // Check for blank lines in the middle of the code.
    checkForBlankLinesInCode(inFilePtr);

    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        if (strlen(label)) {
            instructions[labels].address = address;
            strcpy (instructions[labels].label, label);
            labels++;
            //fprintf(outFilePtr, "%d %s %s %s %s %s\n", address, label, opcode, arg0, arg1, arg2);
        }
        address++;
    }

    for(int i = 0; i < labels - 1; ++i) {
        for(int j = i + 1; j < labels; ++j) {
            if(!strcmp(instructions[i].label, instructions[j].label)) {
                printf("error: there is a label being defined at least twice");
                exit(1);
            }
        }
    }


    rewind(inFilePtr);

    address = 0;
    
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        int val = 0;
        if ((!strcmp(opcode, "add")) || (!strcmp(opcode, "nor"))) {
            val = r_type(arg0, arg1, arg2, opcode, instructions, labels, address);
        } else if((!strcmp(opcode, "lw")) || (!strcmp(opcode, "sw") || (!strcmp(opcode, "beq")))) {
            val = i_type(arg0, arg1, arg2, opcode, instructions, labels, address);
        } else if(!strcmp(opcode, "jalr")) {
            val = j_type(arg0, arg1);
        } else if((!strcmp(opcode, "halt")) || (!strcmp(opcode, "noop"))) {
            val = o_type(opcode);
        } else if(!strcmp(opcode, ".fill")) {
            val = dot_fill(arg0, address, labels, instructions);
        } else {
            printf("%s error: invalid  opcode", opcode);
            exit(1);
        }
        ++address;
        fprintf(outFilePtr, "%d \n", val);
    }
    
    fclose(outFilePtr);

    return(0);
}

// Returns non-zero if the line contains only whitespace.
static int lineIsBlank(char *line) {
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for(int line_idx=0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for(int whitespace_idx = 0; whitespace_idx < 4; ++ whitespace_idx) {
            if(line[line_idx] == whitespace[whitespace_idx]) {
                line_char_is_whitespace = 1;
                break;
            }
        }
        if(!line_char_is_whitespace) {
            nonempty_line = 1;
            break;
        }
    }
    return !nonempty_line;
}

// Exits 2 if file contains an empty line anywhere other than at the end of the file.
// Note calling this function rewinds inFilePtr.
static void checkForBlankLinesInCode(FILE *inFilePtr) {
    char line[MAXLINELENGTH];
    int blank_line_encountered = 0;
    int address_of_blank_line = 0;
    rewind(inFilePtr);

    for(int address = 0; fgets(line, MAXLINELENGTH, inFilePtr) != NULL; ++address) {
        // Check for line too long
        if (strlen(line) >= MAXLINELENGTH-1) {
            printf("error: line too long\n");
            exit(1);
        }

        // Check for blank line.
        if(lineIsBlank(line)) {
            if(!blank_line_encountered) {
                blank_line_encountered = 1;
                address_of_blank_line = address;
            }
        } else {
            if(blank_line_encountered) {
                printf("Invalid Assembly: Empty line at address %d\n", address_of_blank_line);
                exit(2);
            }
        }
    }
    rewind(inFilePtr);
}

/*
* NOTE: The code defined below is not to be modifed as it is implimented correctly.
*/

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0, char *arg1, char *arg2) {
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
	   printf("error: line too long\n");
	   exit(1);
    }

    // Ignore blank lines at the end of the file.
    if(lineIsBlank(line)) {
        return 0;
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);

    return(1);
}

static inline int isNumber(char *string) {
    int num;
    char c;
    return((sscanf(string, "%d%c",&num, &c)) == 1);
}

int offset(char* instruction, struct Symbolic addresses[MAXLINELENGTH], char *arg2, int all_labels, int address) {
    int loc = -1;

    for(int i = 0; i < all_labels; ++i) {
        if(!strcmp(addresses[i].label, arg2)) {
            loc = addresses[i].address;
        }
    }

    
    if(loc == -1) {
        printf("error: label is undefined");
        exit(1);
    }

    if((!strcmp("lw", instruction)) || (!strcmp("sw", instruction))) {
        loc &= 65535;
        return loc;
    }

    if(!strcmp("beq", instruction)) {
        loc = ((loc - address - 1) & 65535);
        return loc;
    }

    if(loc > 65535) {
        printf("offset field is too small");
        exit(1);
    }
    return loc;
}

int r_type(char *arg0, char *arg1, char *arg2, char* type, struct Symbolic addresses[MAXLINELENGTH], int labels, int address) {
    int val = 0;
    
    if(!strcmp(type, "add")) {
        val += 0 << 22;
    } else {
        val += 1 << 22;
    }
    
    int zero = atoi(arg0);
    int one = atoi(arg1);
    int two = atoi(arg2);

    if((zero > 7) || (zero < 0)) {
        printf("register reg0 does not exist");
        exit(1);
    } else if(!isNumber(arg0)) {
        printf("register reg0 is not a number");
        exit(1);
    }

    zero = zero << 19;
    val += zero;

    if((one > 7) || (one < 0)) {
        printf("register reg1 does not exist");
        exit(1);
    } else if(!isNumber(arg1)) {
        printf("register reg1 is not a number");
        exit(1);
    }

    one = one << 16;
    val += one;


    if(isNumber(arg2)) {
        if((two > 7) || (two < 0)) {
            printf("register reg2 does not exist");
            exit(1);
        }
        val += two;
    } else { 
        printf("register reg2 is not a number");
        exit(1);
    }

    return val;
}

int i_type(char *arg0, char *arg1, char *arg2, char* type, struct Symbolic addresses[MAXLINELENGTH], int labels, int address) {
    int val = 0;
    int zero = atoi(arg0);
    int one = atoi(arg1);

    long int two;
    sscanf(arg2, "%ld", &two);

    if(!strcmp(type, "lw")) {
        val += 2 << 22;
    } else if(!strcmp(type, "sw")) {
        val += 3 << 22;
    } else {
        val += 4 << 22;
    }

    if((zero > 7) || (zero < 0)) {
        printf("register reg0 does not exist");
        exit(1);
    } else if(!isNumber(arg0)) {
        printf("register reg0 is not a number");
        exit(1);
    }
    
    zero = zero << 19;
    val += zero;

    if((one > 7) || (one < 0)) {
        printf("register reg1 does not exist");
        exit(1);
    } else if(!isNumber(arg1)) {
        printf("register reg1 is not a number");
        exit(1);
    }

    one = one << 16;
    val += one;
    
    if(isNumber(arg2)) {
        if((two >= -32768) && (two < 32768)) {
            if(two < 0) {
                two = two + 65536;
            }
            val += two;
        } else {
            if(two < -32768) {
                printf("offset field is too small");
            } else {
                printf("offset field is to big");
            }
            exit(1);
        }
    } else {
        two = offset(type, addresses, arg2, labels, address);
        if(two < 65536) {
            val += two;
        } else {
            printf("offset field is too small");
            exit(1);
        }
    }
    
    return val;
}

int j_type(char* arg0, char* arg1) {
    int val = 0;
    int zero = atoi(arg0);
    int one = atoi(arg1);
    val += 5 << 22;

    if((zero > 7) || (zero < 0)) {
        printf("register reg0 does not exist");
        exit(1);
    } else if(!isNumber(arg0)) {
        printf("register reg0 is not a number");
        exit(1);
    }
    
    zero = zero << 19;
    val += zero;

    if((one > 7) || (one < 0)) {
        printf("register reg1 does not exist");
        exit(1);
    } else if(!isNumber(arg1)) {
        printf("register reg1 is not a number");
        exit(1);
    }
    
    one = one << 16;
    val += one;

    return val;
}

int o_type(char* type) {
    int val = 0;
    
    if(!strcmp(type, "halt")) {
        val += 6 << 22;
    } else {
        val += 7 << 22;
    }

    return val;
}

int dot_fill(char* arg0, int address, int labels, struct Symbolic addresses[MAXLINELENGTH]) {
    int val = 0;

    if(isNumber(arg0)) {
        val = atoi(arg0);
        if((atoi(arg0) >= -2147483648) && (atoi(arg0) <= 2147483647)) {
            return val;
        } else {
            if(val < 0) {
                printf("offset field is too small");
                exit(1);
            }
            printf("offset field is tooo big.");
            exit(1);
        }
    }

    return offset(".fill", addresses, arg0, labels, address);
}