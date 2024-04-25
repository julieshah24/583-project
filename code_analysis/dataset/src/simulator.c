/**
 * EECS 370 Project 3
 * Pipeline Simulator
 *
 * This fragment should be used to modify your project 1 simulator to simulator
 * a pipeline
 *
 * Make sure *not* to modify printState or any of the associated functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of data words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

#define ADD 0
#define NOR 1
#define LW 2
#define SW 3
#define BEQ 4
#define JALR 5 /* JALR will not implemented for Project 3 */
#define HALT 6
#define NOOP 7

#define NOOPINSTRUCTION 0x1c00000

typedef struct IFIDStruct {
	int instr;
	int pcPlus1;
} IFIDType;

typedef struct IDEXStruct {
	int instr;
	int pcPlus1;
	int readRegA;
	int readRegB;
	int offset;
} IDEXType;

typedef struct EXMEMStruct {
	int instr;
	int branchTarget;
	int aluResult;
	int readRegB;
} EXMEMType;

typedef struct MEMWBStruct {
	int instr;
	int writeData;
} MEMWBType;

typedef struct WBENDStruct {
	int instr;
	int writeData;
} WBENDType;

typedef struct stateStruct {
	int pc;
	int instrMem[NUMMEMORY];
	int dataMem[NUMMEMORY];
	int reg[NUMREGS];
	int numMemory;
	IFIDType IFID;
	IDEXType IDEX;
	EXMEMType EXMEM;
	MEMWBType MEMWB;
	WBENDType WBEND;
	int cycles; /* number of cycles run so far */
} stateType;

int
field0(int instruction)
{
    return( (instruction>>19) & 0x7);
}

int
field1(int instruction)
{
    return( (instruction>>16) & 0x7);
}

int
field2(int instruction)
{
    return(instruction & 0xFFFF);
}

int
opcode(int instruction)
{
    return(instruction>>22);
}

void
printInstruction(int instr)
{

    char opcodeString[10];

    if (opcode(instr) == ADD) {
        strcpy(opcodeString, "add");
    } else if (opcode(instr) == NOR) {
        strcpy(opcodeString, "nor");
    } else if (opcode(instr) == LW) {
        strcpy(opcodeString, "lw");
    } else if (opcode(instr) == SW) {
        strcpy(opcodeString, "sw");
    } else if (opcode(instr) == BEQ) {
        strcpy(opcodeString, "beq");
    } else if (opcode(instr) == JALR) {
        strcpy(opcodeString, "jalr");
    } else if (opcode(instr) == HALT) {
        strcpy(opcodeString, "halt");
    } else if (opcode(instr) == NOOP) {
        strcpy(opcodeString, "noop");
    } else {
        strcpy(opcodeString, "data");
    }
    printf("%s %d %d %d\n", opcodeString, field0(instr), field1(instr),
        field2(instr));
}

void
printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate before cycle %d starts\n", statePtr->cycles);
    printf("\tpc %d\n", statePtr->pc);

    printf("\tdata memory:\n");
    for (i=0; i<statePtr->numMemory; i++) {
        printf("\t\tdataMem[ %d ] %d\n", i, statePtr->dataMem[i]);
    }
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("\tIFID:\n");
    printf("\t\tinstruction ");
    printInstruction(statePtr->IFID.instr);
    printf("\t\tpcPlus1 %d\n", statePtr->IFID.pcPlus1);
    printf("\tIDEX:\n");
    printf("\t\tinstruction ");
    printInstruction(statePtr->IDEX.instr);
    printf("\t\tpcPlus1 %d\n", statePtr->IDEX.pcPlus1);
    printf("\t\treadRegA %d\n", statePtr->IDEX.readRegA);
    printf("\t\treadRegB %d\n", statePtr->IDEX.readRegB);
    printf("\t\toffset %d\n", statePtr->IDEX.offset);
    printf("\tEXMEM:\n");
    printf("\t\tinstruction ");
    printInstruction(statePtr->EXMEM.instr);
    printf("\t\tbranchTarget %d\n", statePtr->EXMEM.branchTarget);
    printf("\t\taluResult %d\n", statePtr->EXMEM.aluResult);
    printf("\t\treadRegB %d\n", statePtr->EXMEM.readRegB);
    printf("\tMEMWB:\n");
    printf("\t\tinstruction ");
    printInstruction(statePtr->MEMWB.instr);
    printf("\t\twriteData %d\n", statePtr->MEMWB.writeData);
    printf("\tWBEND:\n");
    printf("\t\tinstruction ");
    printInstruction(statePtr->WBEND.instr);
    printf("\t\twriteData %d\n", statePtr->WBEND.writeData);
}

int
convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1 << 15)) {
        num -= (1 << 16);
    }
    return(num);
}


int
main(int argc, char* argv[]) {
    char line[MAXLINELENGTH];
    stateType state;
    stateType newState;
    FILE* filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
        state.numMemory++) {

        if (sscanf(line, "%d", state.instrMem + state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.instrMem[state.numMemory]);
    }

    // copy the instrMem into the dataMem
    // memcpy(state.dataMem, state.instrMem, state.numMemory + 1);
    for (int i = 0; i < state.numMemory; ++i) {
        state.dataMem[i] = state.instrMem[i];
    }

    // variable to track the number of instructions executed
    // not sure if we need this
    // int numInstructions = 0;

    // initialize all registers to 0
    for (int i = 0; i < NUMREGS; ++i) {
        state.reg[i] = 0;
    }

    // initialize the pc to 0
    state.pc = 0;

    // intialize cycles to 0
    state.cycles = 0;

    // initialize the instruction field in all pipeline registers to noop
    // noop = 0x1c00000
    state.IFID.instr = 0x1c00000;
    state.IDEX.instr = 0x1c00000;
    state.EXMEM.instr = 0x1c00000;
    state.MEMWB.instr = 0x1c00000;
    state.WBEND.instr = 0x1c00000;

    // execute instructions while program has not halted
    while (1) {

        printState(&state);

        /* check for halt */
        if (opcode(state.MEMWB.instr) == HALT) {
            printf("machine halted\n");
            printf("total of %d cycles executed\n", state.cycles);
            exit(0);
        }

        newState = state;
        newState.cycles++;

        // state.pc+1
        ++newState.pc;

        /* --------------------- IF stage --------------------- */
        // IF/ID struct contains: instr, pcPlus1
        newState.IFID.instr = state.instrMem[state.pc];
        newState.IFID.pcPlus1 = state.pc + 1;

        /* --------------------- ID stage --------------------- */
        // ID/EX struct contains: instr, pcPlus1, readRegA, readRegB, offset
        newState.IDEX.instr = state.IFID.instr;
        newState.IDEX.pcPlus1 = state.IFID.pcPlus1;

        // check for lw hazard
      
        // variable to track if there is a hazard
        int hazard = 0;
        // if previous instruction was lw
        if (opcode(state.IDEX.instr) == 2) {
            // regB of previous instruction
            int regB = field1(state.IDEX.instr);
            // if current instruction is add
            if (opcode(newState.IDEX.instr) == 0) {
                // add: using value of regA and regB for adding
                int addRegA = field0(newState.IDEX.instr);
                int addRegB = field1(newState.IDEX.instr);
                if (addRegA == regB || addRegB == regB) 
                    hazard = 1;
            }
            // else if current instruction is nor
            else if (opcode(newState.IDEX.instr) == 1) {
                // nor: using value of regA and regB for nor
                int addRegA = field0(newState.IDEX.instr);
                int addRegB = field1(newState.IDEX.instr);
                if (addRegA == regB || addRegB == regB)
                    hazard = 1;
            }
            // else if current instruction is lw
            else if (opcode(newState.IDEX.instr) == 2) {
                // lw: using value of regA 
                int addRegA = field0(newState.IDEX.instr);
                if (addRegA == regB)
                    hazard = 1;
            }
            // else if current instruction is sw
            else if (opcode(newState.IDEX.instr) == 3) {
                // sw: using value of regA and regB for storing
                int addRegA = field0(newState.IDEX.instr);
                int addRegB = field1(newState.IDEX.instr);
                if (addRegA == regB || addRegB == regB)
                    hazard = 1;
            }
            //else if current instruction is beq
            else if (opcode(newState.IDEX.instr) == 4) {
                // beq: using value of regA and regB for comparison
                int addRegA = field0(newState.IDEX.instr);
                int addRegB = field1(newState.IDEX.instr);
                if (addRegA == regB || addRegB == regB)
                    hazard = 1;
            }
        }

        // if hazard detected
        if (hazard == 1) {
            // set ID state to noop
            newState.IDEX.instr = 0x1c00000;
            --newState.pc;
            // stall the ID stage
            newState.IFID.instr = state.IFID.instr;
            newState.IFID.pcPlus1 = state.IFID.pcPlus1;
        }
        
        // no hazards: grab data regularly
        // doesn't matter if we do this when there is a hazard, because
        // ID state is noop so AG won't check the values
        newState.IDEX.readRegA = state.reg[field0(state.IFID.instr)];
        newState.IDEX.readRegB = state.reg[field1(state.IFID.instr)];
        int op = opcode(state.IFID.instr);
        // if lw,sw, or beq: need to use convertNum to get the offset value.
        if (op == 2 || op == 3 || op == 4) {
            newState.IDEX.offset = convertNum(field2(state.IFID.instr));
        }
        

        /* --------------------- EX stage --------------------- */
        // EX/MEM struct contains: instr, branchTarget, aluResult, readRegB
        newState.EXMEM.instr = state.IDEX.instr;
        op = opcode(state.IDEX.instr);
        // check for add/nor hazard

        // one previous instruction ahead
        int instr1 = state.EXMEM.instr;
        // two previous instructions ahead
        int instr2 = state.MEMWB.instr;
        // three previous instructions ahead
        int instr3 = state.WBEND.instr;

        // variable for regA
        int regA = state.IDEX.readRegA;

        // variable for regB
        int regB = state.IDEX.readRegB;

        // check current instruction's regA
       
        // if instr3 is add/nor/lw
        if (opcode(instr3) == 0 || opcode(instr3) == 1 || opcode(instr3) == 2) {
            // grab destReg of instr3
            int destReg = field2(instr3);
            if (opcode(instr3) == 2) {
                destReg = field1(instr3);
            }
            // add: regA 
            // nor: regA 
            // lw: regA
            // sw: regA 
            // beq: regA 
            // regA of current instruction
            int readRegA = field0(newState.EXMEM.instr);
            // if current regA is equal to destReg of previous instruction
            if (readRegA == destReg)
                regA = state.WBEND.writeData;
        }

        // if instr2 is add/nor/lw
        if (opcode(instr2) == 0 || opcode(instr2) == 1 || opcode(instr2) == 2) {
            // grab destReg of instr2
            int destReg = field2(instr2);
            if (opcode(instr2) == 2) {
                destReg = field1(instr2);
            }
            // add: regA
            // nor: regA 
            // lw: regA
            // sw: regA 
            // beq: regA 
            // regA of current instruction
            int readRegA = field0(newState.EXMEM.instr);
            // if current regA is equal to destReg of previous instruction
            if (readRegA == destReg)
                regA = state.MEMWB.writeData;
        }
        
        // if instr1 is add/nor
        if (opcode(instr1) == 0 || opcode(instr1) == 1) {
            // grab destReg of instr1
            int destReg = field2(instr1);
            // don't need to check opcode of current instruction
            // because all instructions use regA
            // add: regA 
            // nor: regA 
            // lw: regA
            // sw: regA 
            // beq: regA 
            // regA of current instruction
            int readRegA = field0(newState.EXMEM.instr);
            // if current regA is equal to destReg of previous instruction
            if (readRegA == destReg) {
                // don't do this
                //state.IDEX.readRegA = state.EXMEM.aluResult;
                regA = state.EXMEM.aluResult;
            }
        }

        //regB
        
        // if instr3 is add/nor/lw
        if (opcode(instr3) == 0 || opcode(instr3) == 1 || opcode(instr3) == 2) {
            // grab destReg of instr3
            int destReg = field2(instr3);
            if (opcode(instr3) == 2) {
                destReg = field1(instr3);
            }
            // check opcode of current instruction
            // add: regB
            // nor: regB
            // lw: DONT CHECK
            // sw: regB
            // beq: regB
            // make sure this is not lw instruction
            if (opcode(newState.EXMEM.instr) == 0 || opcode(newState.EXMEM.instr) == 1 ||
                opcode(newState.EXMEM.instr) == 3 || opcode(newState.EXMEM.instr) == 4) {
                // regB of current instruction
                int readRegB = field1(newState.EXMEM.instr);
                // if current regB is equal to destReg of previous instruction
                if (readRegB == destReg)
                    regB = state.WBEND.writeData;
            }
        }

        // if instr2 is add/nor/lw
        if (opcode(instr2) == 0 || opcode(instr2) == 1 || opcode(instr2) == 2) {
            // grab destReg of instr2
            int destReg = field2(instr2);
            if (opcode(instr2) == 2) {
                destReg = field1(instr2);
            }
            // check opcode of current instruction
            // add: regB
            // nor: regB
            // lw: DONT CHECK
            // sw: regB
            // beq: regB
            // make sure this is not lw instruction
            if (opcode(newState.EXMEM.instr) == 0 || opcode(newState.EXMEM.instr) == 1 ||
                opcode(newState.EXMEM.instr) == 3 || opcode(newState.EXMEM.instr) == 4) {
                // regB of current instruction
                int readRegB = field1(newState.EXMEM.instr);
                // if current regB is equal to destReg of previous instruction
                if (readRegB == destReg) {
                    regB = state.MEMWB.writeData;

                }
            }
        }
        
        // if instr1 is add/nor
        if (opcode(instr1) == 0 || opcode(instr1) == 1) {
            // grab destReg of instr1
            int destReg = field2(instr1);
            // check opcode of current instruction
            // add: regB
            // nor: regB
            // lw: DONT CHECK
            // sw: regB
            // beq: regB
            // make sure this is not lw instruction
            if (opcode(newState.EXMEM.instr) == 0 || opcode(newState.EXMEM.instr) == 1 ||
                opcode(newState.EXMEM.instr) == 3 || opcode(newState.EXMEM.instr) == 4) {
                // regB of current instruction
                int readRegB = field1(newState.EXMEM.instr);
                // if current regB is equal to destReg of previous instruction
                if (readRegB == destReg)
                    regB = state.EXMEM.aluResult;
            }
        }
        

        // add
        if (op == 0) {
            newState.EXMEM.aluResult = regA + regB;
        }
        // nor
        else if (op == 1) {
            newState.EXMEM.aluResult = ~(regA | regB);
        }
        // lw
        else if (op == 2) {
            newState.EXMEM.aluResult = regA + state.IDEX.offset;
        }
        // sw
        else if (op == 3) {
            newState.EXMEM.aluResult = regA + state.IDEX.offset;
            newState.EXMEM.readRegB = regB;
        }
        // beq
        else if (op == 4) {
            // branch will be taken
            if (regA == regB) {
                newState.EXMEM.branchTarget = state.IDEX.pcPlus1 + state.IDEX.offset;
                newState.EXMEM.aluResult = 0;
                newState.EXMEM.readRegB = regB;
            }
            // branch will not be taken
            else {
                newState.EXMEM.branchTarget = state.IDEX.pcPlus1 + state.IDEX.offset;
                newState.EXMEM.aluResult = 1;
                newState.EXMEM.readRegB = regB;
            }
        }
        newState.EXMEM.readRegB = regB;

        /* --------------------- MEM stage --------------------- */
        // check for beq branching in this stage 
        // MEM/WB struct contains: instr, writeData
        newState.MEMWB.instr = state.EXMEM.instr;
        op = opcode(state.EXMEM.instr);
        // add
        if (op == 0) {
            newState.MEMWB.writeData = state.EXMEM.aluResult;
        }
        // nor
        else if (op == 1) {
            newState.MEMWB.writeData = state.EXMEM.aluResult;
        }
        // lw
        else if (op == 2) {
            newState.MEMWB.writeData = state.dataMem[state.EXMEM.aluResult];
        }
        // sw
        else if (op == 3) {
            newState.dataMem[state.EXMEM.aluResult] = state.EXMEM.readRegB;
        }
        // beq
        else if (op == 4) {
            // branch taken
            if (state.EXMEM.aluResult == 0) {
                newState.pc = state.EXMEM.branchTarget;
                // replace instructions in previous stages to noops
                newState.IFID.instr = 0x1c00000;
                newState.IDEX.instr = 0x1c00000;
                newState.EXMEM.instr = 0x1c00000;
            }
            // branch not taken
            else {
                // do nothing
            }
        }

        /* --------------------- WB stage --------------------- */
        // WB/END struct contains: instr, writeData
        newState.WBEND.instr = state.MEMWB.instr;
        newState.WBEND.writeData = state.MEMWB.writeData;
        op = opcode(state.MEMWB.instr);
        // add
        if (op == 0) {
            newState.reg[field2(state.MEMWB.instr)] = state.MEMWB.writeData;
        }
        // nor
        else if (op == 1) {
            newState.reg[field2(state.MEMWB.instr)] = state.MEMWB.writeData;
        }
        // lw
        else if (op == 2) {
            newState.reg[field1(state.MEMWB.instr)] = state.MEMWB.writeData;
            //newState.WBEND.writeData = state.MEMWB.writeData;
        }
        // sw
        else if (op == 3) {
            // do nothing?
            //newState.WBEND.writeData = state.MEMWB.writeData;
        }
        // beq
        else if (op == 4) {
            // do nothing?
            //newState.WBEND.writeData = state.MEMWB.writeData;
        }

        state = newState; /* this is the last statement before end of the loop.
                It marks the end of the cycle and updates the
                current state with the values calculated in this
                cycle */
    }
}

