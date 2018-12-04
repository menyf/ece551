#include <stdio.h>

#include <cstdlib>

#include "shell.h"

int main(int argc, char ** argv) {
  Shell myShell;
  myShell.run();
  return EXIT_SUCCESS;
}
