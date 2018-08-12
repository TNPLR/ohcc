#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "instruction.h"
#include "libc.h"
uint64_t *text;
uint64_t *stack;
uint64_t *data;
uint64_t *bss;
uint64_t ax, bx, cx, *px, *pc, *sp, *bp;
uint64_t *pro_text, *pro_data, *pro_bss;
int virtualMachine();
int readBitCode(FILE *restrict fPtr, uint64_t *restrict text,
    uint64_t *restrict data, uint64_t *restrict bss) {
  uint64_t tmp;
  fread(&tmp, sizeof(uint64_t), 1, fPtr);
  if (tmp != LON) {
    printf("Executive file not correct as OHVM.\n");
    return -1;
  }
  fread(&tmp, sizeof(uint64_t), 1, fPtr);
  pc = text + tmp;

  fread(&pro_text, sizeof(uint64_t), 1, fPtr);
  fread(&pro_data, sizeof(uint64_t), 1, fPtr);
  fread(&pro_bss, sizeof(uint64_t), 1, fPtr);
  printf("%lu %lu %lu\n", (uint64_t)pro_text, (uint64_t)pro_data, (uint64_t)pro_bss);
  fread(&tmp, sizeof(uint64_t), 1, fPtr);
  if (tmp != TXT) {
    printf("Executive file not correct as OHVM.\n");
    return -1;
  }
  fread(&tmp, sizeof(uint64_t), 1, fPtr);
  while (1) {
    if (tmp == TXT) {
      uint64_t tmp2;
      tmp2 = tmp;
      fread(&tmp, sizeof(uint64_t), 1, fPtr);
      if (tmp == TXT) {
        break; // Go to .data
      } else {
        *text++ = tmp2;
        *text++ = tmp;
        fread(&tmp, sizeof(uint64_t), 1, fPtr);
      }
    } else {
      *text++ = tmp;
      fread(&tmp, sizeof(uint64_t), 1, fPtr);
    }
  }
  fread(&tmp, sizeof(uint64_t), 1, fPtr);
  if (tmp != DAT) {
    printf("Executive file not correct as OHVM.\n");
    return -1;
  }
  fread(&tmp, sizeof(uint64_t), 1, fPtr);
  while (1) {
    if (tmp == DAT) {
      uint64_t tmp2;
      tmp2 = tmp;
      fread(&tmp, sizeof(uint64_t), 1, fPtr);
      if (tmp == DAT) {
        break; // Go to .data
      } else {
        *data++ = tmp2;
        *data++ = tmp;
        fread(&tmp, sizeof(uint64_t), 1, fPtr);
      }
    } else {
      *data++ = tmp;
      fread(&tmp, sizeof(uint64_t), 1, fPtr);
    }
  }
  fread(&tmp, sizeof(uint64_t), 1, fPtr);
  if (tmp != BSS) {
    printf("Executive file not correct as OHVM.\n");
    return -1;
  }
  fread(&tmp, sizeof(uint64_t), 1, fPtr);
  while (1) {
    if (tmp == BSS) {
      uint64_t tmp2;
      tmp2 = tmp;
      fread(&tmp, sizeof(uint64_t), 1, fPtr);
      if (tmp == BSS) {
        break; // Go to .data
      } else {
        *bss++ = tmp2;
        *bss++ = tmp;
        fread(&tmp, sizeof(uint64_t), 1, fPtr);
      }
    } else {
      *bss++ = tmp;
      fread(&tmp, sizeof(uint64_t), 1, fPtr);
    }
  }
  return 0;
}
int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Unexpect arguments\n");
    return -1;
  }
  FILE *fPtr;
  fPtr = fopen(argv[1], "rb");
  if (!fPtr) {
    printf("Could not open this file.\n");
    return -1;
  }
  uint64_t poolsize = 4096;
  if (!(text = (uint64_t*)malloc(poolsize))) {
    printf("Could not malloc(%lu) for text area.", poolsize);
    return -1;
  }
  if (!(stack = (uint64_t*)malloc(poolsize))) {
    printf("Could not malloc(%lu) for stack area.", poolsize);
    return -1;
  }
  if (!(data = (uint64_t*)malloc(poolsize))) {
    printf("Could not malloc(%lu) for data area.", poolsize);
    return -1;
  }
  if (!(bss = (uint64_t*)malloc(poolsize))) {
    printf("Could not malloc(%lu) for bss area.", poolsize);
    return -1;
  }

  memset(text, 0, poolsize);
  memset(stack, 0, poolsize);
  memset(data, 0, poolsize);
  memset(bss, 0, poolsize);
  if (readBitCode(fPtr, text, data, bss) == -1) {
    return -1;
  }
  printf("Done\n");
  px = bp = sp = (uint64_t*)((uint64_t)stack + poolsize);
  ax = 0;
  bx = 0;
  cx = 0;
  virtualMachine();
  free(text);
  free(stack);
  free(data);
  free(bss);
  return 0;
}
int virtualMachine() {
  int op;
  //int* tmp;
  while (1) {
    op = *pc++;
    if (op == MAB) {
      bx = ax;
    } else if (op == MAC) {
      cx = ax;
    } else if (op == MBC) {
      cx = bx;
    } else if (op == MCB) {
      bx = cx;
    } else if (op == MCA) {
      ax = cx;
    } else if (op == MBA) {
      ax = bx;
    } else if (op == MAP) {
      px = (uint64_t*)ax;
    } else if (op == ADD) {
      ax = bx + cx;
    } else if (op == SUB) {
      ax = bx - cx;
    } else if (op == ADC) {
      ax = (uint64_t)((int64_t)bx + (int64_t)cx);
    } else if (op == SBC) {
      ax = (uint64_t)((int64_t)bx - (int64_t)cx);
    } else if (op == MUL) {
      ax = bx * cx;
    } else if (op == DIV) {
      ax = bx / cx;
    } else if (op == MOD) {
      ax = bx % cx;
    } else if (op == MLC) {
      ax = (uint64_t)((int64_t)bx * (int64_t)cx);
    } else if (op == DVC) {
      ax = (uint64_t)((int64_t)bx / (int64_t)cx);
    } else if (op == MDC) {
      ax = (uint64_t)((int64_t)bx % (int64_t)cx);
    } else if (op == AND) {
      ax = bx & cx;
    } else if (op == ORR) {
      ax = bx | cx;
    } else if (op == EOR) {
      ax = bx ^ cx;
    } else if (op == SHL) {
      ax = bx << cx;
    } else if (op == SHR) {
      ax = bx >> cx;
    } else if (op == CPL) {
      ax = ~bx;
    } else if (op == CEQ) {
      ax = bx == cx;
    } else if (op == CNE) {
      ax = bx != cx;
    } else if (op == CGT) {
      ax = bx > cx;
    } else if (op == CLT) {
      ax = bx < cx;
    } else if (op == CGE) {
      ax = bx >= cx;
    } else if (op == CLE) {
      ax = bx <= cx;
    } else if (op == NOT) {
      ax = !bx;
    } else if (op == LDI) {
      ax = (uint64_t)(*(int64_t*)px);
    } else if (op == LDC) {
      ax = (uint64_t)(*(char*)px);
    } else if (op == LDU) {
      if (px >= pro_bss) {
        ax = *(uint64_t*)((uint64_t)bss + (uint64_t)px);
      } else if (px >= pro_data) {
        ax = *(uint64_t*)((uint64_t)data + (uint64_t)px);
      } else if (px > pro_text) {
        ax = *(uint64_t*)((uint64_t)text + (uint64_t)px);
      }
    } else if (op == SDR) {
      *px = ax;
    } else if (op == JMP) {
      pc = px;
    } else if (op == JPF) {
      pc = (ax ? (pc + 1) : px);
    } else if (op == JPT) {
      pc = (ax ? px : (pc + 1));
    } else if (op == PSH) {
      *--sp = bx;
    } else if (op == POP) {
      ax = *sp++;
    } else if (op == CAL) {
      *--sp = (uint64_t)(pc+1);
      pc = px;
    } else if (op == ENT) {
      *--sp = (uint64_t)bp;
      bp = sp;
      sp = sp - *pc++;
    } else if (op == ADJ) {
      sp = sp + *pc++;
    } else if (op == LEV) {
      sp = bp;
      bp = (uint64_t*)*sp++;
      pc = (uint64_t*)*sp++;
    } else if (op == LEA) {
      ax = (uint64_t)(bp + *pc++);
    } else if (op == LIC) {
      if (!callLibC(*pc++)) {
        return bx;
      }
    } else if (op == LON) {
      ax = *pc++;
    } else {
      printf("Unknown\n");
      return -1;
    }
  }
  return 0;
}
