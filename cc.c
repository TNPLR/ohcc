#include <stdio.h>
#include <string.h>
enum {
  Num = 256, Fun, Sys, Glo, Loc, Id,
  Char, Else, Enum, Struct, If, Int, Return, Sizeof, While, For, Signed,
  Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr,
  Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};
int token; // current token
char *src, *old_src;
int line;
struct identifier {
  int token;
  int hash;
  char * name;
  int class;
  int type;
  int value;
  int Bclass;
  int Btype;
  int Bvalue;
};
int id_count;
struct identifier *vector_id;
int main(int argc, char* argv[]) {
  // Do nothing now
  id_count = 0;
  vector_id = calloc(256, sizeof(struct identifier));
  free(vector_id);
  return 0;
}

void lex() {
  char *last_pos;
  int hash;

  while (token = *src) {
    ++src;
    if (token == '\n') {
      ++line;
    } else if (token == '#') {
      // It's for preprocessor
      while (*src != 0 && *src != '\n') {
        src++;
      }
    } else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_')) {
      last_pos = src - 1;
      hash = token;
      while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') ||
          (*src >= '0' && *src <= '9') || (*src == '_')) {
        hash = hash * 147 + *src;
        src++;
      }
    }
  }
  return;
}
