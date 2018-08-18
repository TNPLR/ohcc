#ifndef LIBCAS_H_
#define LIBCAS_H_
#include "libcnum.h"
#include <string.h>
int chooseLibC(char *str) {
  if (!strcmp(str, "printf")) {
    return PRINTF;
  } else if (!strcmp(str, "exit")) {
    return EXIT;
  }
  return -1;
}
#endif
