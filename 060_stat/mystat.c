#include <ctype.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// Required header files
#include <string.h>
#include <sys/sysmacros.h>
// Step 1
char * fetchType(mode_t st_mode);
void printInfo(const char * filename, struct stat * sb);

//This function is for Step 4
char * time2str(const time_t * when, long ns);

int main(int argc, char ** argv) {
  struct stat sb;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (lstat(argv[1], &sb) == -1) {
    perror("lstat");
    exit(EXIT_FAILURE);
  }

  printInfo(argv[1], &sb);

  return EXIT_SUCCESS;
}

char * fetchType(mode_t st_mode) {
  switch (st_mode & S_IFMT) {
    case S_IFBLK:
      return strdup("block special file");
    case S_IFCHR:
      return strdup("character special file");
    case S_IFDIR:
      return strdup("directory");
    case S_IFIFO:
      return strdup("fifo");
    case S_IFLNK:
      return strdup("symbolic link");
    case S_IFREG:
      return strdup("regular file");
    case S_IFSOCK:
      return strdup("socket");
    default:
      return strdup("unknown?");
  }
}

void printInfo(const char * filename, struct stat * sb) {
  char * filetype = fetchType(sb->st_mode);

  printf("  File: ‘%s’\n", filename);  // to be proceed.
  printf("  Size: %-10lu\tBlocks: %-10lu IO Block: %-6lu %s\n",
         (unsigned long)sb->st_size,
         (unsigned long)sb->st_blocks,
         (long)sb->st_blksize,
         filetype);
  printf("Device: %lxh/%lud\tInode: %-10lu  Links: %lu\n",
         (long)sb->st_dev,
         (long)sb->st_dev,
         (long)sb->st_ino,
         (long)sb->st_nlink);

  free(filetype);
}

char * time2str(const time_t * when, long ns) {
  char * ans = malloc(128 * sizeof(*ans));
  char temp1[64];
  char temp2[32];
  const struct tm * t = localtime(when);
  strftime(temp1, 512, "%Y-%m-%d %H:%M:%S", t);
  strftime(temp2, 32, "%z", t);
  snprintf(ans, 128, "%s.%09ld %s", temp1, ns, temp2);
  return ans;
}
