#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "libcas.h"
#include "instruction.h"
int assembler(FILE *ftmp, FILE *fout);
int preprocessor(FILE *fin, FILE *ftmp);
void escaped_charactor(char* str) {
  char *tmp, *tmp2;
  char *mem = calloc(strlen(str)+1, sizeof(char));
  printf("%s", str);
  tmp = str;
  while (*str != '\0') {
    if (*str == '\\') {
      if (*++str == 'n') {
        *str = 10;
      }
    }
    ++str;
  }
  str = tmp;
  tmp2 = mem;
  while (*str != '\0') {
    if (*str == '\\') {
      *mem++ = *++str;
      ++str;
    } else {
      *mem++ = *str++;
    }
  }
  memset(tmp, 0, strlen(tmp2)+1);
  printf("%s", tmp2);
  strcpy(tmp, tmp2);
  free(tmp2);
}
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
    printf("FILE cannot write.\n");
    return -1;
  }
  preprocessor(fin, ftmp);
  fclose(ftmp);
  ftmp = fopen(".ohastmp", "r");
  if (!ftmp) {
    printf("File read error.\n");
    return -1;
  }
  //assembler(ftmp, fout);
  assembler(ftmp, fout);
  fclose(fin);
  fclose(fout);
  return 0;
}
struct label {
  char name[32];
  uint64_t position;
};
struct label label_array[128];
int label_count = 0;
int preprocessor(FILE *fin, FILE *ftmp) {
  uint64_t count = 0;
  char read[256];
  while ((fscanf(fin, "%s", read)) != EOF) {
    if (!strcmp(read, "LON")) {
      fscanf(fin, "%*s");
      count += 2;
    } else if (!strcmp(read, "ASCII")) {
      fscanf(fin, "%s \"%*[A-Za-z ]\"", label_array[label_count].name);
      label_array[label_count].position = count;
      printf("%s:%lu\n", label_array[label_count].name, count);
      count += strlen(label_array[label_count].name) + 1;
      ++label_count;
    } else if (!strcmp(read, "LABEL")) {
      fscanf(fin, "%s", label_array[label_count].name);
      printf("%s:%lu\n", label_array[label_count].name, count);
      label_array[label_count].position = count;
      ++label_count;
    } else if (!strcmp(read, "QUAD")) {
      fscanf(fin, "%s %*u", label_array[label_count].name);
      printf("%s:%lu\n", label_array[label_count].name, count);
      label_array[label_count].position = count;
      ++count;
      ++label_count;
    } else if (!(strcmp(read, "TXT") &&
        strcmp(read, "DAT") &&
        strcmp(read, "BSS"))) {
      continue;
    } else {
      ++count;
    }
  }
  rewind(fin);
  int check = 0;
  uint64_t tmp_num;
  while ((fscanf(fin, "%s", read)) != EOF) {
    fprintf(ftmp, "%s\n", read);
    if (!strcmp(read, "LON")) {
      fscanf(fin, "%s", read);
      for (int i = 0; i < label_count; ++i) {
        if (!strcmp(read, label_array[i].name)) {
          fprintf(ftmp, "%lu\n", (uint64_t)((uint64_t*)0+label_array[i].position));
          printf("%lu\n", (uint64_t)((uint64_t*)0+label_array[i].position));
          check = 1;
          break;
        }
      }
      if (check == 0) {
        sscanf(read, "%lu", &tmp_num);
        fprintf(ftmp, "%lu\n", tmp_num);
      }
      check = 0;
    } else if (!strcmp(read, "ASCII")) {
      char name_[32];
      fscanf(fin, "%s \"%[A-z ]\"", name_, read);
      fprintf(ftmp, "%s\n\"%s\"\n", name_, read);
    } else if (!strcmp(read, "QUAD")) {
      char name_[32];
      fscanf(fin, "%s %s", name_, read);
      fprintf(ftmp, "%s\n", name_);
      for (int i = 0; i < label_count; ++i) {
        if (!strcmp(read, label_array[i].name)) {
          fprintf(ftmp, "%lu\n", (uint64_t)((uint64_t*)0+label_array[i].position));
          printf("%lu\n", (uint64_t)((uint64_t*)0+label_array[i].position));
          check = 1;
          break;
        }
      }
      if (check == 0) {
        sscanf(read, "%lu", &tmp_num);
        fprintf(ftmp, "%lu\n", tmp_num);
      }
      check = 0;
    }
  }
  return 0;
}
int assembler(FILE *ftmp, FILE *fout) {
  uint64_t tmp;
  char read[256];
  while ((fscanf(ftmp, "%s", read)) != EOF) {
    printf("%s\n", read);
    if (!strcmp(read, "SOP")) {
      tmp = SOP; 
    } else if (!strcmp(read, "TXT")) {
      tmp = TXT;
    } else if (!strcmp(read, "DAT")) {
      tmp = DAT;
    } else if (!strcmp(read, "BSS")) {
      tmp = BSS;
    } else if (!strcmp(read, "MAB")) {
      tmp = MAB;
    } else if (!strcmp(read, "MAC")) {
      tmp = MAC;
    } else if (!strcmp(read, "MBC")) {
      tmp = MBC;
    } else if (!strcmp(read, "MCB")) {
      tmp = MCB;
    } else if (!strcmp(read, "MCA")) {
      tmp = MCA;
    } else if (!strcmp(read, "MBA")) {
      tmp = MBA;
    } else if (!strcmp(read, "MAP")) {
      tmp = MAP;
    } else if (!strcmp(read, "ADD")) {
      tmp = ADD;
    } else if (!strcmp(read, "SUB")) {
      tmp = SUB;
    } else if (!strcmp(read, "ADC")) {
      tmp = ADC;
    } else if (!strcmp(read, "SBC")) {
      tmp = SBC;
    } else if (!strcmp(read, "MUL")) {
      tmp = MUL;
    } else if (!strcmp(read, "DIV")) {
      tmp = DIV;
    } else if (!strcmp(read, "MOD")) {
      tmp = MOD;
    } else if (!strcmp(read, "MLC")) {
      tmp = MLC;
    } else if (!strcmp(read, "DVC")) {
      tmp = DVC;
    } else if (!strcmp(read, "MDC")) {
      tmp = MDC;
    } else if (!strcmp(read, "AND")) {
      tmp = AND;
    } else if (!strcmp(read, "ORR")) {
      tmp = ORR;
    } else if (!strcmp(read, "EOR")) {
      tmp = EOR;
    } else if (!strcmp(read, "SHL")) {
      tmp = SHL;
    } else if (!strcmp(read, "SHR")) {
      tmp = SHR;
    } else if (!strcmp(read, "CPL")) {
      tmp = CPL;
    } else if (!strcmp(read, "CEQ")) {
      tmp = CEQ;
    } else if (!strcmp(read, "CNE")) {
      tmp = CNE;
    } else if (!strcmp(read, "CGT")) {
      tmp = CGT;
    } else if (!strcmp(read, "CLT")) {
      tmp = CLT;
    } else if (!strcmp(read, "CGE")) {
      tmp = CGE;
    } else if (!strcmp(read, "CLE")) {
      tmp = CLE;
    } else if (!strcmp(read, "NOT")) {
      tmp = NOT;
    } else if (!strcmp(read, "LDI")) {
      tmp = LDI;
    } else if (!strcmp(read, "LDC")) {
      tmp = LDC;
    } else if (!strcmp(read, "LDU")) {
      tmp = LDU;
    } else if (!strcmp(read, "SDR")) {
      tmp = SDR;
    } else if (!strcmp(read, "JMP")) {
      tmp = JMP;
    } else if (!strcmp(read, "JPF")) {
      tmp = JPF;
    } else if (!strcmp(read, "JPT")) {
      tmp = JPT;
    } else if (!strcmp(read, "PSH")) {
      tmp = PSH;
    } else if (!strcmp(read, "POP")) {
      tmp = POP;
    } else if (!strcmp(read, "CAL")) {
      tmp = CAL;
    } else if (!strcmp(read, "ENT")) {
      tmp = ENT;
    } else if (!strcmp(read, "ADJ")) {
      tmp = ADJ;
    } else if (!strcmp(read, "LEV")) {
      tmp = LEV;
    } else if (!strcmp(read, "LEA")) {
      tmp = LEA;
    } else if (!strcmp(read, "LIC")) {
      tmp = LIC;
      fwrite(&tmp, sizeof(uint64_t), 1, fout);
      fscanf(ftmp, "%s", read);
      tmp = chooseLibC(read);
    } else if (!strcmp(read, "LON")) {
      tmp = LON;
      fwrite(&tmp, sizeof(uint64_t), 1, fout);
      uint64_t tread = 0;
      fscanf(ftmp, "%lu", &tread);
      fwrite(&tread, sizeof(uint64_t), 1, fout);
      continue;
    } else if (!strcmp(read, "ASCII")) {
      memset(read, 0, 256);
      fscanf(ftmp, "%*s\n\"%[A-Za-z\\ ]\"", read);
      escaped_charactor(read);
      fwrite(read, sizeof(char), sizeof(uint64_t)*(strlen(read)/sizeof(uint64_t) + 1) , fout);
      continue;
    } else if (!strcmp(read, "BYTE")) {
      continue;
    } else if (!strcmp(read, "WORD")) {
      continue;
    } else if (!strcmp(read, "DWORD")) {
      continue;
    } else if (!strcmp(read, "QUAD")) {
      fscanf(ftmp, "%*s\n%lu", &tmp);
    } else if (!strcmp(read, "LABEL")) {
      fscanf(ftmp, "%*s");
      continue;
    } else {
      printf("Error\n");
      return -1;
    }
    fwrite(&tmp, sizeof(uint64_t), 1, fout);
    fflush(fout);
  }
  return 0;
}
