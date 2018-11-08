#include "cstdlib"
#include "dedup.h"

int main(int argc, char ** argv) {
  Dedup dd;
  for (int i = 1; i < argc; i++) {
    dd.run(argv[i]);
  }
  dd.print();
  return EXIT_SUCCESS;
}
