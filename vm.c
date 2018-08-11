#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
uint64_t *text;
uint64_t *stack;
uint64_t *data;
uint64_t *bss;
uint64_t ax, bx, cx, *px, *pc, *sp, *bp;
// instuctions
enum {
  MAB,MAC,MBC,MCB,MCA,MBA,MAP,ADD,SUB,ADC,SBC,MUL,DIV,MOD,MLC,DVC,MDC,
  AND,ORR,EOR,SHL,SHR,CPL,CEQ,CNE,CGT,CLT,CGE,CLE,NOT,LDI,LDC,LDU,SDR,
  JMP,JPF,JPT,PSH,POP,CAL,ENT,ADJ,LEV,LEA,LIC,LON
};
enum {
  PRINTF,EXIT
};
int virtualMachine();
int callLibC(int num) {
  switch(num) {
    case PRINTF:
      ax = printf("%s",px);
      break;
    case EXIT:
      ax = printf("exit(%d)\n", bx);
      return 0;
    default:
      break;
  }
  return 1;
}
int main(int argc, char* argv[]) {
  uint64_t poolsize = 4096;
  if (!(text = (uint64_t*)malloc(poolsize))) {
    printf("Could not malloc(%d) for text area.", poolsize);
    return -1;
  }
  if (!(stack = (uint64_t*)malloc(poolsize))) {
    printf("Could not malloc(%d) for stack area.", poolsize);
    return -1;
  }
  if (!(data = (uint64_t*)malloc(poolsize))) {
    printf("Could not malloc(%d) for data area.", poolsize);
    return -1;
  }
  if (!(bss = (uint64_t*)malloc(poolsize))) {
    printf("Could not malloc(%d) for bss area.", poolsize);
    return -1;
  }

  memset(text, 0, poolsize);
  memset(stack, 0, poolsize);
  memset(data, 0, poolsize);
  memset(bss, 0, poolsize);

  px = bp = sp = (uint64_t*)((uint64_t)stack + poolsize);
  ax = 0;
  bx = 0;
  cx = 0;
  virtualMachine();
  return 0;
}
int virtualMachine() {
  int op, *tmp;
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
      ax = bx && cx;
    } else if (op == ORR) {
      ax = bx || cx;
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
      ax = *px;
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
