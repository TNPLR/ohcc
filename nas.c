#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ncpu.h"
enum {
  BYTE=0xF0,WORD,DWORD,QUAD,RESB,RESW,RESD,RESQ,LABEL
};
struct command {
  uint64_t cmd;
  uint64_t pos;
  char type;
};
struct variable {
  char name[32];
  char command[256];
  uint64_t cmd;
  uint64_t pos;
};
typedef struct variable VAR;
typedef struct command CMD;
VAR pro_var[16384];
CMD program[16384];
int var_count = 0;
int pro_count = 0;
uint64_t pos = 0;
void print_pass_one(FILE *fin) {
  fprintf(stdout, "****************PASS ONE****************\n");
  char tmp[256];
  for (int i = 0; i < pro_count; ++i) {
    fgets(tmp,255,fin);
    tmp[strcspn(tmp,"\n")] = 0;
    fprintf(stdout, "%04lx\t%s\t\t%02lx\n",program[i].pos, tmp, program[i].cmd);
  }
}
void print_symbol() {
  fprintf(stdout, "**************SYMBOL TABLE**************\n");
  for (int i = 0; i < var_count; ++i) {
    pro_var[i].command[strcspn(pro_var[i].command, "\n")] = 0;
    fprintf(stdout, "%04lx\t%s\t\t%02lx\n",pro_var[i].pos, pro_var[i].command, pro_var[i].cmd);
  }
}
uint64_t readRegister(char* str) {
  uint64_t tmp = 0;
  sscanf(str, "%*c%lu", &tmp);
  printf("tmp:%lu\n",tmp);
  return tmp;
}
uint64_t convertToVBit(uint64_t cmd, char type, char* str) {
  uint64_t Rtmp[3];
  uint64_t const_value;
  char A[5][5];
  if (type == 'R') {
    sscanf(str, "%*s %3[^,]%*c %3[^,]%*c %3[^,]", A[0], A[1], A[2]);
    Rtmp[0] = readRegister(A[0])&0xF;
    Rtmp[1] = readRegister(A[1])&0xF;
    Rtmp[2] = readRegister(A[2])&0xF;
    printf("%s, %s, %s\n", A[0], A[1], A[2]);
    printf("%lu, %lu, %lu\n", Rtmp[0], Rtmp[1], Rtmp[2]);
    return ((cmd<<56)|(Rtmp[0]<<52)|(Rtmp[1]<<48)|(Rtmp[2]<<44));
  } else if (type == 'L') {
    sscanf(str, "%*s %3[^,]%*c %3[^,]%*c %lu", A[0], A[1], &const_value);
    Rtmp[0] = readRegister(A[0])&0xF;
    Rtmp[1] = readRegister(A[1])&0xF;
    return (cmd<<56)|(Rtmp[0]<<52)|(Rtmp[1]<<48)|(const_value&0xFFFFFFFFFFFF);
  } else if (type == 'J') {
    sscanf(str, "%*s %lu", &const_value);
    return (cmd<<56)|(const_value&0xFFFFFFFFFFFFFF);
  } else if (type == 'D') {
    return 0;
  }
  return 0;
}
void pass_two(FILE *fin, FILE *fout) {
  char tmp[256];
  uint64_t now_ins;
  for (int i = 0; i < pro_count; ++i) {
    fgets(tmp, 255, fin);
    fprintf(stdout, "...");
    now_ins = convertToVBit(program[i].cmd, program[i].type, tmp);
    fwrite(&now_ins, sizeof(uint64_t), 1, fout);
  }
}
void pos_check(char* line) {
  char tmp[256];
  sscanf(line, "%s", tmp);
  if (!strcmp(tmp, "LDQ")) {
    program[pro_count].cmd = LDQ;
    program[pro_count].type = 'L';
  } else if (!strcmp(tmp, "LDD")) {
    program[pro_count].cmd = LDD;
    program[pro_count].type = 'L';
  } else if (!strcmp(tmp, "LDW")) {
    program[pro_count].cmd = LDW;
    program[pro_count].type = 'L';
  } else if (!strcmp(tmp, "LDB")) {
    program[pro_count].cmd = LDB;
    program[pro_count].type = 'L';
  } else if (!strcmp(tmp, "STQ")) {
    program[pro_count].cmd = STQ;
    program[pro_count].type = 'L';
  } else if (!strcmp(tmp, "STD")) {
    program[pro_count].cmd = STD;
    program[pro_count].type = 'L';
  } else if (!strcmp(tmp, "STW")) {
    program[pro_count].cmd = STW;
    program[pro_count].type = 'L';
  } else if (!strcmp(tmp, "STB")) {
    program[pro_count].cmd = STB;
    program[pro_count].type = 'L';
  } else if (!strcmp(tmp, "LDRQ")) {
    program[pro_count].cmd = LDRQ;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "LDRD")) {
    program[pro_count].cmd = LDRD;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "LDRW")) {
    program[pro_count].cmd = LDRW;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "LDRB")) {
    program[pro_count].cmd = LDRB;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "STRQ")) {
    program[pro_count].cmd = STRQ;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "STRD")) {
    program[pro_count].cmd = STRD;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "STRW")) {
    program[pro_count].cmd = STRW;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "STRB")) {
    program[pro_count].cmd = STRB;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "LDI")) {
    program[pro_count].cmd = LDI;
    program[pro_count].type = 'L';
  } else if (!strcmp(tmp, "CMP")) {
    program[pro_count].cmd = CMP;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "MOV")) {
    program[pro_count].cmd = MOV;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "ADD")) {
    program[pro_count].cmd = ADD;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "SUB")) {
    program[pro_count].cmd = SUB;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "MUL")) {
    program[pro_count].cmd = MUL;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "DIV")) {
    program[pro_count].cmd = DIV;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "ADC")) {
    program[pro_count].cmd = ADC;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "SBC")) {
    program[pro_count].cmd = SBC;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "MLC")) {
    program[pro_count].cmd = MLC;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "DVC")) {
    program[pro_count].cmd = DVC;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "AND")) {
    program[pro_count].cmd = AND;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "ORR")) {
    program[pro_count].cmd = ORR;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "EOR")) {
    program[pro_count].cmd = EOR;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "ROL")) {
    program[pro_count].cmd = ROL;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "ROR")) {
    program[pro_count].cmd = ROR;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "SHL")) {
    program[pro_count].cmd = SHL;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "SHR")) {
    program[pro_count].cmd = SHR;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "JEQ")) {
    program[pro_count].cmd = JEQ;
    program[pro_count].type = 'J';
  } else if (!strcmp(tmp, "JNE")) {
    program[pro_count].cmd = JNE;
    program[pro_count].type = 'J';
  } else if (!strcmp(tmp, "JLT")) {
    program[pro_count].cmd = JLT;
    program[pro_count].type = 'J';
  } else if (!strcmp(tmp, "JGT")) {
    program[pro_count].cmd = JGT;
    program[pro_count].type = 'J';
  } else if (!strcmp(tmp, "JLE")) {
    program[pro_count].cmd = JLE;
    program[pro_count].type = 'J';
  } else if (!strcmp(tmp, "JGE")) {
    program[pro_count].cmd = JGE;
    program[pro_count].type = 'J';
  } else if (!strcmp(tmp, "JMP")) {
    program[pro_count].cmd = JMP;
    program[pro_count].type = 'J';
  } else if (!strcmp(tmp, "CALL")) {
    program[pro_count].cmd = CALL;
    program[pro_count].type = 'J';
  } else if (!strcmp(tmp, "RET")) {
    program[pro_count].cmd = RET;
    program[pro_count].type = 'J';
  } else if (!strcmp(tmp, "INT")) {
    program[pro_count].cmd = INT;
    program[pro_count].type = 'J';
  } else if (!strcmp(tmp, "PUSHQ")) {
    program[pro_count].cmd = PUSHQ;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "PUSHD")) {
    program[pro_count].cmd = PUSHD;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "PUSHW")) {
    program[pro_count].cmd = PUSHW;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "PUSHB")) {
    program[pro_count].cmd = PUSHB;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "POPQ")) {
    program[pro_count].cmd = POPQ;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "POPD")) {
    program[pro_count].cmd = POPD;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "POPW")) {
    program[pro_count].cmd = POPW;
    program[pro_count].type = 'R';
  } else if (!strcmp(tmp, "POPB")) {
    program[pro_count].cmd = POPB;
    program[pro_count].type = 'R';
  } else {
    tmp[strcspn(tmp,":")] = 0;
    strcpy(pro_var[var_count].name, tmp);
    strcpy(pro_var[var_count].command, line);
    program[pro_count].pos = pos;
    pro_var[var_count].pos = pos;
    sscanf(line, "%*s %s", tmp);
    uint64_t res = 0;
    if (!strcmp(tmp, "BYTE")) {
      program[pro_count].cmd = BYTE;
      program[pro_count].type = 'D';
      pro_var[var_count].cmd = BYTE;
      ++pos;
    } else if (!strcmp(tmp, "WORD")) {
      program[pro_count].cmd = WORD;
      program[pro_count].type = 'D';
      pro_var[var_count].cmd = WORD;
      pos += 2;
    } else if (!strcmp(tmp, "DWORD")) {
      program[pro_count].cmd = DWORD;
      program[pro_count].type = 'D';
      pro_var[var_count].cmd = DWORD;
      pos += 4;
    } else if (!strcmp(tmp, "QUAD")) {
      program[pro_count].cmd = QUAD;
      program[pro_count].type = 'D';
      pro_var[var_count].cmd = QUAD;
      pos += 8;
    } else if (!strcmp(tmp, "RESB")) {
      program[pro_count].cmd = RESB;
      program[pro_count].type = 'D';
      pro_var[var_count].cmd = RESB;
      sscanf(line, "%*s %*s %lu", &res);
      pos += res;
    } else if (!strcmp(tmp, "RESW")) {
      program[pro_count].cmd = RESW;
      program[pro_count].type = 'D';
      pro_var[var_count].cmd = RESW;
      sscanf(line, "%*s %*s %lu", &res);
      pos += res*2;
    } else if (!strcmp(tmp, "RESD")) {
      program[pro_count].cmd = RESD;
      program[pro_count].type = 'D';
      pro_var[var_count].cmd = RESD;
      sscanf(line, "%*s %*s %lu", &res);
      pos += res*4;
    } else if (!strcmp(tmp, "RESQ")) {
      program[pro_count].cmd = RESQ;
      program[pro_count].type = 'D';
      pro_var[var_count].cmd = RESQ;
      sscanf(line, "%*s %*s %lu", &res);
      pos += res*8;
    } else if (!strcmp(tmp, "LABEL")) {
      program[pro_count].cmd = LABEL;
      program[pro_count].type = 'D';
      pro_var[var_count].cmd = LABEL;
    } else {
      fprintf(stdout, "Unknown instruction\n");
    }
    ++var_count;
    ++pro_count;
    return;
  }
  program[pro_count].pos = pos;
  ++pro_count;
  pos += 8;
}
void assembly(char* in, char* out) {
  FILE *fin, *fout;
  fin = fopen(in, "r");
  fout = fopen(out, "wb");
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
  print_symbol();
  rewind(fin);
  pass_two(fin, fout);
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
