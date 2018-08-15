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
  haltVM();
  return 0;
}
