#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

int printfile(const char *path) {
  FILE *status = fopen(path, "r");
  if (status == NULL) {
    fprintf(stderr, "error opening file %s: %s (%d)\n", path, strerror(errno), errno);
    return 0;
  }

  char buf[BUFSIZ + 1];
  buf[BUFSIZ] = 0;

  bool reading = true;      
  while (reading) {
    int read = fread(buf, sizeof(char), sizeof(char) * BUFSIZ, status);

    if (read != BUFSIZ) {
      if (ferror(status) != 0) {
        fprintf(stderr, "error reading file %s:  %s (%d)\n", path, strerror(errno), errno);
        return 0;
      }

      // EOF
      reading = false;
      fclose(status);
      buf[read] = 0;
    }
    
    printf("%s", buf);
  }

  return 1;
}