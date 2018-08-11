#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "instruction.h"
int assembler(FILE *fin, FILE *fout);
int preprocessor(FILE *fin, FILE *ftmp);
int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Bad arguments\n");
    return -1;
  }
  FILE *fin, *fout, *ftmp;
  fin = fopen(argv[1], "r");
  fout = fopen(argv[2], "wb");
  ftmp = fopen(".ohastmp", "w");
  if (!fin) {
    printf("File read fail.\n");
    return -1;
  }
  if (!fout) {
    printf("File cannot write.\n");
    return -1;
  }
  if (!ftmp) {
    printf("TMP file of preprocessor error.\n");
    exit(-1);
  }
  //preprocessor(fin, ftmp);
  assembler(ftmp, fout);
  fclose(fin);
  fclose(fout);
  return 0;
}
struct label {
  char name[32];
  uint64_t* position;
};
struct label label_array[128];
int label_count = 0;
int preprocessor(FILE *fin, FILE *ftmp) {
  uint64_t* count = 0;
  char read[32];
  while ((fscanf(fin, "%s", read)) != EOF) {
    if (strcmp(read, "LON")) {
      count += 2;
    } else if (strcmp(read, "ASCII")) {
      fscanf(fin, "%s", label_array[label_count].name);
      label_array[label_count].position = count;
      count += strlen(label_array[label_count].name) + 1;
      ++label_count;
    } else if (strcmp(read, "LABEL")) {
      fscanf(fin, "%s", label_array[label_count].name);
      label_array[label_count].position = count;
      ++label_count;
    } else {
      ++count;
    }
  }
  rewind(fin);
  while ((fscanf(fin, "%s", read)) != EOF) {
    fprintf(ftmp, "%s", read);
    if (strcmp(read, "LON")) {
      for (int i = 0; i < label_count; ++i) {
        if (strcmp(read, label_array[label_count].name)) {
          //fprintf("%d")
        }
      }
    }
  }
  return 0;
}
int assembler(FILE *fin, FILE *fout) {
  uint64_t tmp;
  char read[32];
  while ((fscanf(fin, "%s", read)) != EOF) {
    if (strcmp(read, "SOP")) {
      tmp = SOP; 
    } else if (strcmp(read, "TXT")) {
      tmp = TXT;
    } else if (strcmp(read, "DAT")) {
      tmp = DAT;
    } else if (strcmp(read, "BSS")) {
      tmp = BSS;
    } else if (strcmp(read, "MAB")) {
      tmp = MAB;
    } else if (strcmp(read, "MAC")) {
      tmp = MAC;
    } else if (strcmp(read, "MBC")) {
      tmp = MBC;
    } else if (strcmp(read, "MCB")) {
      tmp = MCB;
    } else if (strcmp(read, "MCA")) {
      tmp = MCA;
    } else if (strcmp(read, "MBA")) {
      tmp = MBA;
    } else if (strcmp(read, "MAP")) {
      tmp = MAP;
    } else if (strcmp(read, "ADD")) {
      tmp = ADD;
    } else if (strcmp(read, "SUB")) {
      tmp = SUB;
    } else if (strcmp(read, "ADC")) {
      tmp = ADC;
    } else if (strcmp(read, "SBC")) {
      tmp = SBC;
    } else if (strcmp(read, "MUL")) {
      tmp = MUL;
    } else if (strcmp(read, "DIV")) {
      tmp = DIV;
    } else if (strcmp(read, "MOD")) {
      tmp = MOD;
    } else if (strcmp(read, "MLC")) {
      tmp = MLC;
    } else if (strcmp(read, "DVC")) {
      tmp = DVC;
    } else if (strcmp(read, "MDC")) {
      tmp = MDC;
    } else if (strcmp(read, "AND")) {
      tmp = AND;
    } else if (strcmp(read, "ORR")) {
      tmp = ORR;
    } else if (strcmp(read, "EOR")) {
      tmp = EOR;
    } else if (strcmp(read, "SHL")) {
      tmp = SHL;
    } else if (strcmp(read, "SHR")) {
      tmp = SHR;
    } else if (strcmp(read, "CPL")) {
      tmp = CPL;
    } else if (strcmp(read, "CEQ")) {
      tmp = CEQ;
    } else if (strcmp(read, "CNE")) {
      tmp = CNE;
    } else if (strcmp(read, "CGT")) {
      tmp = CGT;
    } else if (strcmp(read, "CLT")) {
      tmp = CLT;
    } else if (strcmp(read, "CGE")) {
      tmp = CGE;
    } else if (strcmp(read, "CLE")) {
      tmp = CLE;
    } else if (strcmp(read, "NOT")) {
      tmp = NOT;
    } else if (strcmp(read, "LDI")) {
      tmp = LDI;
    } else if (strcmp(read, "LDC")) {
      tmp = LDC;
    } else if (strcmp(read, "LDU")) {
      tmp = LDU;
    } else if (strcmp(read, "SDR")) {
      tmp = SDR;
    } else if (strcmp(read, "JMP")) {
      tmp = JMP;
    } else if (strcmp(read, "JPF")) {
      tmp = JPF;
    } else if (strcmp(read, "JPT")) {
      tmp = JPT;
    } else if (strcmp(read, "PSH")) {
      tmp = PSH;
    } else if (strcmp(read, "POP")) {
      tmp = POP;
    } else if (strcmp(read, "CAL")) {
      tmp = CAL;
    } else if (strcmp(read, "ENT")) {
      tmp = ENT;
    } else if (strcmp(read, "ADJ")) {
      tmp = ADJ;
    } else if (strcmp(read, "LEV")) {
      tmp = LEV;
    } else if (strcmp(read, "LEA")) {
      tmp = LEA;
    } else if (strcmp(read, "LIC")) {
      tmp = LIC;
    } else if (strcmp(read, "LON")) {
      tmp = LON;
      fwrite(&tmp, sizeof(uint64_t), 1, fout);
      uint64_t tread = 0;
      fscanf(fin, "%lu", &tread);
      fwrite(&tread, sizeof(uint64_t), 1, fout);
      continue;
    } else if (strcmp(read, "ASCII")) {
      tmp = TXT;
    } else if (strcmp(read, "BYTE")) {
      tmp = TXT;
    } else if (strcmp(read, "WORD")) {
      tmp = TXT;
    } else if (strcmp(read, "DWORD")) {
      tmp = TXT;
    } else if (strcmp(read, "QUAD")) {
      tmp = TXT;
    } else if (strcmp(read, "LABEL")) {
      tmp = TXT;
    }
    fwrite(&tmp, sizeof(uint64_t), 1, fout);
  }
  return 0;
}
