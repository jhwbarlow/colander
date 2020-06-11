#include <ctype.h>

int isnumericstring(const char *str) {
  while (*str != 0) {
    if (!isdigit(*str)) {
      return 0;
    }

    str++;
  }

  return 1;
}