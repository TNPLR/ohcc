#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "libc.h"
#include "libcnum.h"
extern uint64_t ax, bx, cx, *px, *pc, *sp, *bp, *pro_text, *pro_data, *pro_bss;
extern uint64_t* segment_mem_pos(uint64_t*);
int callLibC(uint64_t num) {
  switch(num) {
    case PRINTF:
      ax = printf("%s",(char*)segment_mem_pos(px));
      break;
    case EXIT:
      ax = bx;
      return 0;
    default:
      break;
  }
  return 1;
}
