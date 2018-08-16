/************************************************
 * OH Virtual Machine
 * Copyright TNPLR 2018
 *
 * This virtual machine can run OHVM bit code
 * If have any problem, email me:
 *   hsiaosvideo@gmail.com
************************************************/
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
uint64_t cmpValue(uint64_t R) {
  R >>= 62;
  return R;
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
      l_type_instruction(op_register, op_const);
      R[op_register[0]] = R[op_register[1]] + op_const;
    } else if (op == CMP) {
      r_type_instruction(op_register, op_const);
      R[12] = R[12] & 0x3FFFFFFFFFFFFFFF;
      if (R[op_register[0]] > R[op_register[1]]) {
        R[12] |= 0x8000000000000000;
      } else if (R[op_register[0]] == R[op_register[1]]) {
        R[12] |= 0xC000000000000000;
      } else if (R[op_register[0]] == R[op_register[1]]) {
        R[12] |= 0x4000000000000000;
      }
    } else if (op == MOV) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = R[op_register[1]];
    } else if (op == ADD) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = R[op_register[1]] + R[op_register[2]];
    } else if (op == SUB) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = R[op_register[1]] - R[op_register[2]];
    } else if (op == MUL) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = R[op_register[1]] * R[op_register[2]];
    } else if (op == DIV) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = R[op_register[1]] / R[op_register[2]];
    } else if (op == ADC) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = (uint64_t)((int64_t)R[op_register[1]] + (int64_t)R[op_register[2]]);
    } else if (op == SBC) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = (uint64_t)((int64_t)R[op_register[1]] - (int64_t)R[op_register[2]]);
    } else if (op == MLC) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = (uint64_t)((int64_t)R[op_register[1]] * (int64_t)R[op_register[2]]);
    } else if (op == DVC) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = (uint64_t)((int64_t)R[op_register[1]] / (int64_t)R[op_register[2]]);
    } else if (op == AND) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = R[op_register[1]] & R[op_register[2]];
    } else if (op == ORR) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = R[op_register[1]] | R[op_register[2]];
    } else if (op == EOR) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = R[op_register[1]] ^ R[op_register[2]];
    } else if (op == ROL) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = (R[op_register[1]] >> (64 - R[op_register[2]])) |
                          (R[op_register[1]] << R[op_register[2]]);
    } else if (op == ROR) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = (R[op_register[1]] << (64 - R[op_register[2]])) |
                          (R[op_register[1]] >> R[op_register[2]]);
    } else if (op == SHL) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = R[op_register[1]] << R[op_register[2]];
    } else if (op == SHR) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = R[op_register[1]] >> R[op_register[2]];
    } else if (op == JEQ) {
      j_type_instruction(op_const);
      if (cmpValue(R[12]) == 0x3) {
        R[15] += (int64_t)op_const;
      }
    } else if (op == JNE) {
      j_type_instruction(op_const);
      if (cmpValue(R[12]) != 0x3) {
        R[15] += (int64_t)op_const;
      }
    } else if (op == JLT) {
      j_type_instruction(op_const);
      if (cmpValue(R[12]) == 0x1) {
        R[15] += (int64_t)op_const;
      }
    } else if (op == JGT) {
      j_type_instruction(op_const);
      if (cmpValue(R[12]) == 0x2) {
        R[15] += (int64_t)op_const;
      }
    } else if (op == JLE) {
      j_type_instruction(op_const);
      if (cmpValue(R[12]) != 0x2) {
        R[15] += (int64_t)op_const;
      }
    } else if (op == JGE) {
      j_type_instruction(op_const);
      if (cmpValue(R[12]) != 0x1) {
        R[15] += (int64_t)op_const;
      }
    } else if (op == JMP) {
      j_type_instruction(op_const);
      R[15] += (int64_t)op_const;
    } else if (op == CALL) {
      j_type_instruction(op_const);
      R[14] = R[15];
      R[15] += (int64_t)op_const;
    } else if (op == RET) {
      j_type_instruction(op_const);
      R[15] = R[14];
    } else if (op == INT) {
      /* Not doing anything now */
    } else if (op == PUSHQ) {
      r_type_instruction(op_register, op_const);
      R[13] -= sizeof(uint64_t);
      *(uint64_t*)R[13] = R[op_register[0]];
    } else if (op == PUSHD) {
      r_type_instruction(op_register, op_const);
      R[13] -= sizeof(uint32_t);
      *(uint32_t*)R[13] = (uint32_t)R[op_register[0]];
    } else if (op == PUSHW) {
      r_type_instruction(op_register, op_const);
      R[13] -= sizeof(uint16_t);
      *(uint16_t*)R[13] = (uint16_t)R[op_register[0]];
    } else if (op == PUSHB) {
      r_type_instruction(op_register, op_const);
      R[13] -= sizeof(uint8_t);
      *(uint8_t*)R[13] = (uint8_t)R[op_register[0]];
    } else if (op == POPQ) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = *(uint64_t*)R[13];
      R[13] += sizeof(uint64_t);
    } else if (op == POPD) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = *(uint32_t*)R[13];
      R[13] += sizeof(uint32_t);
    } else if (op == POPW) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = *(uint16_t*)R[13];
      R[13] += sizeof(uint16_t);
    } else if (op == POPB) {
      r_type_instruction(op_register, op_const);
      R[op_register[0]] = *(uint8_t*)R[13];
      R[13] += sizeof(uint8_t);
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
