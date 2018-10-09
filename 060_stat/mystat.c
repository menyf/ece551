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
// Step 1
char * fetchType(mode_t st_mode);
void printGeneralInfo(const char * filename, struct stat * sb);

// Step 2
void printAccessInfo(struct stat * sb);
char * proceedAccess(struct stat * sb);

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

  printGeneralInfo(argv[1], &sb);
  printAccessInfo(&sb);
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

void printGeneralInfo(const char * filename, struct stat * sb) {
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

char * proceedAccess(struct stat * sb) {
  char * str = malloc(11 * sizeof(*str));

  switch (sb->st_mode & S_IFMT) {
    case S_IFBLK:
      str[0] = 'b';
      break;
    case S_IFCHR:
      str[0] = 'c';
      break;
    case S_IFDIR:
      str[0] = 'd';
      break;
    case S_IFIFO:
      str[0] = 'p';
      break;
    case S_IFLNK:
      str[0] = 'l';
      break;
    case S_IFREG:
      str[0] = '-';
      break;
    case S_IFSOCK:
      str[0] = 's';
      break;
    default:
      str[0] = '?';
      break;
  }

  const mode_t mask[] = {
      S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
  const char identifier[] = "rwx";

  for (int i = 0; i < 9; i++) {
    str[i + 1] = sb->st_mode & mask[i] ? identifier[i % 3] : '-';
  }

  str[10] = 0;
  return str;
}

void printAccessInfo(struct stat * sb) {
  char * access = proceedAccess(sb);
  printf("Access: (%04o/%s)\n", sb->st_mode & ~S_IFMT, access);
  free(access);
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
