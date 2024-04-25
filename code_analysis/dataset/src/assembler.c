/**
 * Project 1 
 * Assembler code fragment for LC-2K 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);

int
main(int argc, char *argv[])
{

    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

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
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */
    // While loop to read everything
    //changed from if !
    // 
    // create an array (?) and store labels. how to search?
    // 2D char array. length max size of file by 7
    // address index for first pass

    // things to look for:
    // is label defined twice?
    // no labels at all?
    // if defined but not used, that's fine

    // variable to track label address
    int counter = 0;
    
    struct table {
        // labels can only be 7 characters long
        char label[7];
    };

    // create array to store labels
    struct table arr_label[100]; // max line length
    
    // first pass! look for labels
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        if (strcmp("", label) != 0) {
            // error check: if label already exists
            for (int i = 0; i < 100; ++i) {
                if (strcmp(arr_label[i].label, label) == 0) {
                    printf("error: duplicate label\n");
                    exit(1);
                }
            }

            // copy label into array 
            strcpy(arr_label[counter].label, label);
        }
        // update address number
        ++counter;
    }
    /* reached end of file */

    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);

    // variable to store pc value
    int pc = 0;

    // fprintf to write to a file

    // second pass! process instructions
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        // brute force all possible opcodes
        // opcodes that can have labels: fill, lw, sw, beq
        // beq: calculate offset
        if (strcmp(opcode, "add") == 0) {
            int base = 0x00000000;
            int op_bin = 0;
            op_bin = op_bin << 22;
            int regA = atoi(arg0);
            int regB = atoi(arg1);
            int destReg = atoi(arg2);
            regA = regA << 19;
            regB = regB << 16;
            base = base | op_bin | regA | regB | destReg;
            fprintf(outFilePtr,"%d%c", base, '\n');
        }
        else if (strcmp(opcode, "nor") == 0) {
            int base = 0x00000000;
            int op_bin = 1;
            op_bin = op_bin << 22;
            int regA = atoi(arg0);
            int regB = atoi(arg1);
            int destReg = atoi(arg2);
            regA = regA << 19;
            regB = regB << 16;
            base = base | op_bin | regA | regB | destReg;
            fprintf(outFilePtr, "%d%c", base, '\n');
        }
        else if (strcmp(opcode, "lw") == 0) {
            int base = 0x00000000;
            int op_bin = 2;
            op_bin = op_bin << 22;
            int regA = atoi(arg0);
            int regB = atoi(arg1);
            regA = regA << 19;
            regB = regB << 16;
            //int offset_base = 0x0000;
            int offset = 0;
            // if offset is a number
            if (isNumber(arg2) == 1) {
                offset = atoi(arg2);
                // error checking: offset fits in 16 bits
                if (offset < -32768 || offset > 32767) {
                    printf("error: offset cannot fit in 16 bits\n");
                    exit(1);
                }
                // if offset is negative, chop bits 
                if (offset < 0) {
                    int mask = 0x0000ffff;
                    offset = offset & mask;
                }
                //offset = offset_base | offset;
            }
            // else offset refers to a label 
            else {
                // go through the label array and search for label
                int label_found = 1;
                for (int i = 0; i < 100; ++i) {
                    if (strcmp(arr_label[i].label, arg2) == 0) {
                        label_found = 0;
                        offset = i;
                    }
                }
                // error checking: offset fits in 16 bits
                if (offset < -32768 || offset > 32767) {
                    printf("error: offset cannot fit in 16 bits\n");
                    exit(1);
                }
                // error check: label not found
                if (label_found == 1) {
                    printf("error: undefined label\n");
                    exit(1);
                }
                //offset = offset_base | offset;
            }
            base = base | op_bin | regA | regB | offset;
            fprintf(outFilePtr, "%d%c", base, '\n');
        }
        else if (strcmp(opcode, "sw") == 0) {
            int base = 0x00000000;
            int op_bin = 3;
            op_bin = op_bin << 22;
            int regA = atoi(arg0);
            int regB = atoi(arg1);
            regA = regA << 19;
            regB = regB << 16;
            //int offset_base = 0x0000;
            int offset = 0;
            // if offset is a number
            if (isNumber(arg2) == 1) {
                offset = atoi(arg2);
                if (offset < -32768 || offset > 32767) {
                    printf("error: offset cannot fit in 16 bits\n");
                    exit(1);
                }
                // if offset is negative, chop bits 
                if (offset < 0) {
                    int mask = 0x0000ffff;
                    offset = offset & mask;
                }
                //offset = offset_base | offset;
            }
            // else offset refers to a label 
            else {
                // go through the label array and search for label
                int label_found = 1;
                for (int i = 0; i < 100; ++i) {
                    if (strcmp(arr_label[i].label, arg2) == 0) {
                        label_found = 0;
                        offset = i;
                    }
                }
                // error checking: offset fits in 16 bits
                if (offset < -32768 || offset > 32767) {
                    printf("error: offset cannot fit in 16 bits\n");
                    exit(1);
                }
                // error check: undefined label
                if (label_found == 1) {
                    printf("error: undefined label\n");
                    exit(1);
                }
                //offset = offset_base | offset;
            }
            base = base | op_bin | regA | regB | offset;
            fprintf(outFilePtr, "%d%c", base, '\n');
        }
        else if (strcmp(opcode, "beq") == 0) {
        // don't need to do any chopping if the offset is positive
            int base = 0x00000000;
            int op_bin = 4;
            op_bin = op_bin << 22;
            int regA = atoi(arg0);
            int regB = atoi(arg1);
            regA = regA << 19;
            regB = regB << 16;
            //int offset_base = 0x00000000;
            int offset = 0;
            int destAd = 0;
            // if offset is a number
            if (isNumber(arg2) == 1) {
                offset = atoi(arg2);
                if (offset < -32768 || offset > 32767) {
                    printf("error: offset cannot fit in 16 bits\n");
                    exit(1);
                }
                // if offset is negative, chop bits 
                if (offset < 0) {
                    int mask = 0x0000ffff;
                    offset = offset & mask;
                }
                //offset = offset_base | offset;
            }
            // else offset refers to a label 
            else {
                // go through the label array and search for label
                int label_found = 1;
                for (int i = 0; i < 100; ++i) {
                    if (strcmp(arr_label[i].label, arg2) == 0) {
                        label_found = 0;
                        destAd = i;
                    }
                }
                // error check: undefined label
                if (label_found == 1) {
                    printf("error: undefined label\n");
                    exit(1);
                }
                offset = destAd - (pc + 1);
                if (offset < -32768 || offset > 32767) {
                    printf("error: offset cannot fit in 16 bits\n");
                    exit(1);
                }
                if (offset < 0) {
                    //printf("%x%c", offset, '\n');
                    int mask = 0x0000ffff;
                    offset = offset & mask;
                    //offset = offset << 16;
                    //printf("%x%c", offset, '\n');
                    // added
                    //printf("%x%c", offset, '\n');
                }
                //offset = offset_base | offset;
            }
            base = base | op_bin | regA | regB | offset;
            fprintf(outFilePtr, "%d%c", base, '\n');
        }
        else if (strcmp(opcode, "jalr") == 0) {
            int base = 0x00000000;
            int op_bin = 5;
            op_bin = op_bin << 22;
            int regA = atoi(arg0);
            int regB = atoi(arg1);
            regA = regA << 19;
            regB = regB << 16;
            base = base | op_bin | regA | regB;
            fprintf(outFilePtr, "%d%c", base, '\n');
        }
        else if (strcmp(opcode, "halt") == 0) {
            int base = 0x00000000;
            int op_bin = 6;
            op_bin = op_bin << 22;
            op_bin = base | op_bin;
            fprintf(outFilePtr, "%d%c", op_bin, '\n');
        }
        else if (strcmp(opcode, "noop") == 0) {
            int base = 0x00000000;
            int op_bin = 7;
            op_bin = op_bin << 22;
            op_bin = base | op_bin;
            fprintf(outFilePtr, "%d%c", op_bin, '\n');
        }
        else if (strcmp(opcode, ".fill") == 0) {
            // if argument is a number
            if (isNumber(arg0) == 1) {
                fprintf(outFilePtr, "%s%c", arg0, '\n');
            }
            // if argument is a label address
            else {
                // go through the label array and search for label
                int label_found = 1;
                for (int i = 0; i < 100; ++i) {
                    if (strcmp(arr_label[i].label, arg0) == 0) {
                        fprintf(outFilePtr, "%d%c", i, '\n');
                        label_found = 0;
                    }
                }
                // error check: label not found
                if (label_found == 1) {
                    printf("error: undefined label\n");
                    exit(1);
                }
            }
        }
        //not a valid opcode, throw an error
        else {
            printf("error: not valid opcode\n");
            exit(1);
        }

        //increment address
        ++pc;
    }



    /* after doing a readAndParse, you may want to do the following to test the
        opcode */
    //if (!strcmp(opcode, "add")) {
        /* do whatever you need to do for opcode "add" */
    //}

    // finished! 
    exit(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if successfully read
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
        char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
        /* reached end of file */
        return(0);
    }

    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL) {
        /* line too long */
        printf("error: line too long\n");
        exit(1);
    }

    /* is there a label? */
    char *ptr = line;
    if (sscanf(ptr, "%[^\t\n\r ]", label)) {
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

int
isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}


