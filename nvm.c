#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
enum instructions {
  LDQ,LDD,LDW,LDB,STQ,STD,STW,STB,LDRQ,LDRD,LDRW,LDRB,
  STRQ,STRD,STRW,STRB,LDI,CMP=0x20,MOV,ADD,SUB,MUL,DIV,
  AND,ORR,EOR,ROL,ROR,SHL,SHR,JEQ=0x40,JNE,JLT,JGT,JLE,
  JGE,JMP,CALL,RET,INT,PUSHQ=0x50,PUSHD,PUSHW,PUSHB,POPQ,
  POPD,POPW,POPB
};
uint8_t *program;
uint64_t R[16];
uint64_t text;
uint64_t data;
uint64_t bss;
uint64_t heap;
uint64_t stack;
int virtualMachine() {
  /* Nothing */
  int op;
  while (1) {
    R[11] = *(uint64_t*)R[15];
    R[15] = (uint64_t*)R[15] + 1;
    op = R[11] >> 56;
    
    if (op == LDQ) {

    } else if (op == LDD) {

    } else if (op == LDW) {

    } else if (op == LDB) {

    } else if (op == STQ) {

    } else if (op == STD) {

    } else if (op == STW) {

    } else if (op == STB) {

    } else if (op == LDRQ) {

    } else if (op == LDRD) {

    } else if (op == LDRW) {

    } else if (op == LDRB) {

    } else if (op == STRQ) {

    } else if (op == STRD) {

    } else if (op == STRW) {

    } else if (op == STRB) {

    } else if (op == CMP) {

    } else if (op == MOV) {

    } else if (op == ADD) {

    } else if (op == SUB) {

    } else if (op == MUL) {

    } else if (op == DIV) {

    } else if (op == AND) {

    } else if (op == ORR) {

    } else if (op == EOR) {

    } else if (op == ROL) {

    } else if (op == ROR) {

    } else if (op == SHL) {

    } else if (op == SHR) {

    } else if (op == JEQ) {

    } else if (op == JNE) {

    } else if (op == JLT) {

    } else if (op == JGT) {

    } else if (op == JLE) {

    } else if (op == JGE) {

    } else if (op == JMP) {

    } else if (op == CALL) {

    } else if (op == RET) {

    } else if (op == INT) {

    } else if (op == PUSHQ) {

    } else if (op == PUSHD) {

    } else if (op == PUSHW) {

    } else if (op == PUSHB) {

    } else if (op == POPQ) {

    } else if (op == POPD) {

    } else if (op == POPW) {

    } else if (op == POPB) {

    } else {
      fprintf(stderr, "Error: Unknown Instruction Error\n");
      return 1;
    }
  }
  return 0;
}
int initVM(char *filename) {
  FILE *fexe;
  uint64_t poolsize = 0;
  fexe = fopen(filename, "rb");
  if (!fexe) {
    fprintf(stderr, "IO Error: Cannot Read %d\n", filename);
    return 1;
  }
  fread(&poolsize, sizeof(uint64_t), 1, fexe);
  if (poolsize == 0) {
    fprintf(stderr, "Pool Size Error: Request %d Bytes Failed\n", poolsize);
    return 1;
  }
  if (!(program = malloc(poolsize))) {
    fprintf(stderr, "Pool Size Error: Request %d Bytes Failed\n", poolsize);
    return 1;
  }
  fread(&text, sizeof(uint64_t), 1, fexe);
  fread(&data, sizeof(uint64_t), 1, fexe);
  fread(&bss, sizeof(uint64_t), 1, fexe);
  fread(&heap, sizeof(uint64_t), 1, fexe);
  fread(&stack, sizeof(uint64_t), 1, fexe);
  stack += heap;
  fread(&R[15], sizeof(uint64_t), 1, fexe);
  R[15] += (uint64_t)program;
  fread(&R[0], sizeof(uint64_t), 1, fexe);
  fread(&R[0], sizeof(uint64_t), 1, fexe);
  R[0] = 0;
  R[12] = 0;
  R[14] = R[13] = stack;
  char *read_program = program;
  fread(read_program++, sizeof(char), 1, fexe);
  while(!(feof(fexe))) {
    fread(read_program++, sizeof(char), 1, fexe);
  }
  fclose(fexe);
  return 0;
}
int haltVM() {
  free(program);
  return 0;
}
int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Arguments not correct. Require 1 Argument\n");
    return 1;
  }
  if (initVM(argv[1])) {
    fprintf(stderr, "Virtual Machine Terminated\n");
    return 1;
  }
  virtualMachine();
  haltVM();
  return 0;
}
