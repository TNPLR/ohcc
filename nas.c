#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ncpu.h"
struct command {
  uint64_t cmd;
  uint64_t pos;
};
typedef struct command CMD;
CMD program[16384];
int pro_count = 0;
uint64_t pos = 0;
void print_pass_one(FILE *fin) {
  char tmp[256];
  for (int i = 0; i < pro_count; ++i) {
    fgets(tmp,255,fin);
    tmp[strcspn(tmp,"\n")] = 0;
    fprintf(stdout, "%04x\t%s\t\t%02x\n",program[i].pos, tmp, program[i].cmd);
  }
}
void pos_check(char* line) {
  char tmp[256];
  sscanf(line, "%s", tmp);
  if (!strcmp(tmp, "LDQ")) {
    program[pro_count].cmd = LDQ;
  } else if (!strcmp(tmp, "LDD")) {
    program[pro_count].cmd = LDD;
  } else if (!strcmp(tmp, "LDW")) {
    program[pro_count].cmd = LDW;
  } else if (!strcmp(tmp, "LDB")) {
    program[pro_count].cmd = LDB;
  } else if (!strcmp(tmp, "STQ")) {
    program[pro_count].cmd = STQ;
  } else if (!strcmp(tmp, "STD")) {
    program[pro_count].cmd = STD;
  } else if (!strcmp(tmp, "STW")) {
    program[pro_count].cmd = STW;
  } else if (!strcmp(tmp, "STB")) {
    program[pro_count].cmd = STB;
  } else if (!strcmp(tmp, "LDRQ")) {
    program[pro_count].cmd = LDRQ;
  } else if (!strcmp(tmp, "LDRD")) {
    program[pro_count].cmd = LDRD;
  } else if (!strcmp(tmp, "LDRW")) {
    program[pro_count].cmd = LDRW;
  } else if (!strcmp(tmp, "LDRB")) {
    program[pro_count].cmd = LDRB;
  } else if (!strcmp(tmp, "STRQ")) {
    program[pro_count].cmd = STRQ;
  } else if (!strcmp(tmp, "STRD")) {
    program[pro_count].cmd = STRD;
  } else if (!strcmp(tmp, "STRW")) {
    program[pro_count].cmd = STRW;
  } else if (!strcmp(tmp, "STRB")) {
    program[pro_count].cmd = STRB;
  } else if (!strcmp(tmp, "LDI")) {
    program[pro_count].cmd = LDI;
  } else if (!strcmp(tmp, "CMP")) {
    program[pro_count].cmd = CMP;
  } else if (!strcmp(tmp, "MOV")) {
    program[pro_count].cmd = MOV;
  } else if (!strcmp(tmp, "ADD")) {
    program[pro_count].cmd = ADD;
  } else if (!strcmp(tmp, "SUB")) {
    program[pro_count].cmd = SUB;
  } else if (!strcmp(tmp, "MUL")) {
    program[pro_count].cmd = MUL;
  } else if (!strcmp(tmp, "DIV")) {
    program[pro_count].cmd = DIV;
  } else if (!strcmp(tmp, "ADC")) {
    program[pro_count].cmd = ADC;
  } else if (!strcmp(tmp, "SBC")) {
    program[pro_count].cmd = SBC;
  } else if (!strcmp(tmp, "MLC")) {
    program[pro_count].cmd = MLC;
  } else if (!strcmp(tmp, "DVC")) {
    program[pro_count].cmd = DVC;
  } else if (!strcmp(tmp, "AND")) {
    program[pro_count].cmd = AND;
  } else if (!strcmp(tmp, "ORR")) {
    program[pro_count].cmd = ORR;
  } else if (!strcmp(tmp, "EOR")) {
    program[pro_count].cmd = EOR;
  } else if (!strcmp(tmp, "ROL")) {
    program[pro_count].cmd = ROL;
  } else if (!strcmp(tmp, "ROR")) {
    program[pro_count].cmd = ROR;
  } else if (!strcmp(tmp, "SHL")) {
    program[pro_count].cmd = SHL;
  } else if (!strcmp(tmp, "SHR")) {
    program[pro_count].cmd = SHR;
  } else if (!strcmp(tmp, "JEQ")) {
    program[pro_count].cmd = JEQ;
  } else if (!strcmp(tmp, "JNE")) {
    program[pro_count].cmd = JNE;
  } else if (!strcmp(tmp, "JLT")) {
    program[pro_count].cmd = JLT;
  } else if (!strcmp(tmp, "JGT")) {
    program[pro_count].cmd = JGT;
  } else if (!strcmp(tmp, "JLE")) {
    program[pro_count].cmd = JLE;
  } else if (!strcmp(tmp, "JGE")) {
    program[pro_count].cmd = JGE;
  } else if (!strcmp(tmp, "JMP")) {
    program[pro_count].cmd = JMP;
  } else if (!strcmp(tmp, "CALL")) {
    program[pro_count].cmd = CALL;
  } else if (!strcmp(tmp, "RET")) {
    program[pro_count].cmd = RET;
  } else if (!strcmp(tmp, "INT")) {
    program[pro_count].cmd = INT;
  } else if (!strcmp(tmp, "PUSHQ")) {
    program[pro_count].cmd = PUSHQ;
  } else if (!strcmp(tmp, "PUSHD")) {
    program[pro_count].cmd = PUSHD;
  } else if (!strcmp(tmp, "PUSHW")) {
    program[pro_count].cmd = PUSHW;
  } else if (!strcmp(tmp, "PUSHB")) {
    program[pro_count].cmd = PUSHB;
  } else if (!strcmp(tmp, "POPQ")) {
    program[pro_count].cmd = POPQ;
  } else if (!strcmp(tmp, "POPD")) {
    program[pro_count].cmd = POPD;
  } else if (!strcmp(tmp, "POPW")) {
    program[pro_count].cmd = POPW;
  } else if (!strcmp(tmp, "POPB")) {
    program[pro_count].cmd = POPB;
  } else {
    fprintf(stdout, "ERROR: Unknown instruction: %s", tmp);
  }
  program[pro_count].pos = pos;
  ++pro_count;
  pos += 8;
}
void assembly(char* in, char* out) {
  FILE *fin, *fout;
  fin = fopen(in, "r");
  fout = fopen(out, "w");
  if (!fin) {
    fprintf(stdout, "Cannot read %s\n", in);
  }
  if (!fout) {
    fprintf(stdout, "Cannot read %s\n", out);
  }
  char tmp_in[256];
  while (fgets(tmp_in, 255, fin) != NULL) {
    pos_check(tmp_in);
  }
  rewind(fin);
  print_pass_one(fin);
  fclose(fin);
  fclose(fout);
}
int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stdout, "Error: Cannot Read File\n");
    return 1;
  }
  assembly(argv[1], argv[2]);
}
