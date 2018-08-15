#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
enum instructions {
  LDQ,LDD,LDW,LDB,STQ,STD,STW,STB,LDRQ,LDRD,LDRW,LDRB,
  STRQ,STRD,STRW,STRB,LDI,CMP=0x20,MOV,ADD,SUB,MUL,DIV,
  ADC,SBC,MLC,DVC,AND,ORR,EOR,ROL,ROR,SHL,SHR,JEQ=0x40,
  JNE,JLT,JGT,JLE,JGE,JMP,CALL,RET,INT,PUSHQ=0x50,PUSHD,
  PUSHW,PUSHB,POPQ,POPD,POPW,POPB
};
uint8_t *program;
uint64_t R[16];
uint64_t text;
uint64_t data;
uint64_t bss;
uint64_t heap;
uint64_t stack;
inline uint64_t bitmask(int bits) {
  return (0xFFFFFFFFFFFFFFFF >> (64 - bits));
}
inline uint64_t readBits(uint64_t input, int bit_start, int bit_end) {
  input >>= bit_end;
  return (input & bitmask(bit_start - bit_end + 1));
}
void r_type_instruction(uint64_t* op_register, uint64_t op_const) {
  op_register[0] = readBits(R[11], 55, 52);
  op_register[1] = readBits(R[11], 51, 48);
  op_register[2] = readBits(R[11], 47, 44);
  op_const = readBits(R[11], 43, 0);
}
void l_type_instruction(uint64_t* op_register, uint64_t op_const) {
  op_register[0] = readBits(R[11], 55, 52);
  op_register[1] = readBits(R[11], 51, 48);
  op_const = readBits(R[11], 47, 0);
}
void j_type_instruction(uint64_t op_const) {
  op_const = readBits(R[11], 55, 0);
}
int virtualMachine() {
  int op;
  uint64_t op_register[3];
  uint64_t op_const;
  while (1) {
    R[11] = *(uint64_t*)R[15];
    R[15] = (uint64_t)((uint64_t*)R[15] + 1);
    op = R[11] >> 56;
    if (op == LDQ) {
      l_type_instruction(op_register, op_const);
      R[op_register[0]] = *(uint64_t*)(R[op_register[1]] + op_const);
    } else if (op == LDD) {
      l_type_instruction(op_register, op_const);
      R[op_register[0]] = *(uint32_t*)(R[op_register[1]] + op_const);
    } else if (op == LDW) {
      l_type_instruction(op_register, op_const);
      R[op_register[0]] = *(uint16_t*)(R[op_register[1]] + op_const);
    } else if (op == LDB) {
      l_type_instruction(op_register, op_const);
      R[op_register[0]] = *(uint8_t*)(R[op_register[1]] + op_const);
    } else if (op == STQ) {
      l_type_instruction(op_register, op_const);
      *(uint64_t*)(R[op_register[1]] + op_const) = R[op_register[0]];
    } else if (op == STD) {
      l_type_instruction(op_register, op_const);
      *(uint32_t*)(R[op_register[1]] + op_const) = (uint32_t) R[op_register[0]];
    } else if (op == STW) {
      l_type_instruction(op_register, op_const);
      *(uint16_t*)(R[op_register[1]] + op_const) = (uint16_t) R[op_register[0]];
    } else if (op == STB) {
      l_type_instruction(op_register, op_const);
      *(uint8_t*)(R[op_register[1]] + op_const) = (uint8_t) R[op_register[0]];
    } else if (op == LDRQ) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = *(uint64_t*)(R[op_register[1]] + R[op_register[2]]);
    } else if (op == LDRD) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = *(uint32_t*)(R[op_register[1]] + R[op_register[2]]);
    } else if (op == LDRW) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = *(uint16_t*)(R[op_register[1]] + R[op_register[2]]);
    } else if (op == LDRB) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = *(uint8_t*)(R[op_register[1]] + R[op_register[2]]);
    } else if (op == STRQ) {
      r_type_instruction(op_register, op_const);
      *(uint64_t*)(R[op_register[1]] + R[op_register[2]]) = R[op_register[0]];
    } else if (op == STRD) {
      r_type_instruction(op_register, op_const);
      *(uint32_t*)(R[op_register[1]] + R[op_register[2]]) = (uint32_t) R[op_register[0]];
    } else if (op == STRW) {
      r_type_instruction(op_register, op_const);
      *(uint16_t*)(R[op_register[1]] + R[op_register[2]]) = (uint16_t) R[op_register[0]];
    } else if (op == STRB) {
      r_type_instruction(op_register, op_const);
      *(uint8_t*)(R[op_register[1]] + R[op_register[2]]) = (uint8_t) R[op_register[0]];
    } else if (op == LDI) {
      l_type_instriction(op_register, op_const);
      R[op_register[0]] = R[op_register[1]] + op_const;
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
