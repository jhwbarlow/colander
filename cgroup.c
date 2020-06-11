#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#define CGROUP_PATH "/sys/fs/cgroup/memory"

int cgroupsenabled(void) {
  struct stat statbuf;

  if (stat(CGROUP_PATH, &statbuf) == -1) {
    if (errno != ENOENT) {
      fprintf(stderr, "error statting dir %s: %s (%d)\n", CGROUP_PATH, strerror(errno), errno);
      return -1;
    }

    // Entry does not exist - no cgroups
    return 0;
  }

  // Check entry is a dir
  if ((statbuf.st_mode & S_IFMT) == S_IFDIR) {
    return 1;
  } else {
    // Entry exists but is not a dir
    return 0;
  }
}