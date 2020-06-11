#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include "printfile.h"
#include "mallinfo.h"
#include "validate.h"
#include "cgroup.h"

#define MEGABYTE (1024 * 1024)

int main(int argc, char *argv[]) {
  int delay, sleep_time, increment, max_mem = -1;

  int opt;
  bool fail = false;
  while ((opt = getopt(argc, argv, "d:s:i:m:")) != -1) {
    switch (opt) {
    case 'd':
      if (!isnumericstring(optarg)) {
        fail = true;
      }

      delay = atoi(optarg);
      if (delay < 0) {
        fail = true;
      }

      if (fail) {
        fprintf(stderr, "invalid delay\n");
        return EXIT_FAILURE;
      }

      break;
    case 's':
      if (!isnumericstring(optarg)) {
        fail = true;
      }

      sleep_time = atoi(optarg);
      if (sleep_time < 0) {
        fail = true;
      }

      if (fail) {
        fprintf(stderr, "invalid sleep time\n");
        return EXIT_FAILURE;
      }

      break;
    case 'i':
      if (!isnumericstring(optarg)) {
        fail = true;
      }

      increment = atoi(optarg);
      if (increment < 1) {
        fail = true;
      }

      if (fail) {
        fprintf(stderr, "invalid increment\n");
        return EXIT_FAILURE;
      }

      break;
    case 'm':
      if (!isnumericstring(optarg)) {
        fail = true;
      }

      max_mem = atoi(optarg);
      if (max_mem < 1) {
        fail = true;
      }

      if (fail) {
        fprintf(stdout, "invalid max mem\n");
        return EXIT_FAILURE;
      }

      break;
    default:
      // ignore
      break;
    }
  }

  if (delay == -1 || sleep_time == -1 || increment == -1 || max_mem == -1) {
    fprintf(stderr, "Usage: %s -d delay -s sleep_time -i increment -m max_mem\n", argv[0]);
    return EXIT_FAILURE;
  }

  bool cgroupsenab = false;

  int cgroups = cgroupsenabled();
  if (cgroups == -1) {
    fprintf(stderr, "could not determine cgroup presence");
  } else if (cgroups == 1) {
    cgroupsenab = true;
  }

  int current = 0;
  for (int i = 0; ; i++) {
    sleep(delay);
    
    printf("==[Round %d]=====================================================================\n", i);

    printf("--[/proc/self/status]-----------------------------------------------------------\n");
    if (!printfile("/proc/self/status")) {
      fprintf(stderr, "could not print status");
    }

    printf("--[mallinfo]--------------------------------------------------------------------\n");
    printmallinfo();

    printf("--[/proc/meminfo]---------------------------------------------------------------\n");
    if (!printfile("/proc/meminfo")) {
      fprintf(stderr, "could not print meminfo");
    }    

    if (cgroupsenab) {
      printf("--[/sys/fs/cgroup/memory/memory.stat]-------------------------------------------\n");

      if (!printfile("/sys/fs/cgroup/memory/memory.stat")) {
        fprintf(stderr, "could not print cgroup mem stats");
      }
    }    

    putchar('\n');
    fflush(stdout);
    sleep(sleep_time);

    if (current >= max_mem) {
      continue;
    }

    int mem_sz = MEGABYTE * increment;
    size_t sz = mem_sz;
    char *mem = (char *) malloc(sz);
    if (mem == NULL) {
      fprintf(stderr, "unable to malloc: %s (%d)\n", strerror(errno), errno);
      return EXIT_FAILURE;
    }

    // Actually touch the memory so it counts as used, not just (over)commited by the kernel
    for (int i = 0, j = 0; i < sz; i++) {
      *(mem + i) = 'A';
    }

    current += mem_sz / MEGABYTE;
  }

  return EXIT_SUCCESS;
}