#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "libc.h"
extern uint64_t ax, bx, cx, *px, *pc, *sp, *bp;
int callLibC(uint64_t num) {
  switch(num) {
    case PRINTF:
      ax = printf("%s",(char*)px);
      break;
    case EXIT:
      ax = printf("exit(%lu)\n", bx);
      return 0;
    default:
      break;
  }
  return 1;
}
