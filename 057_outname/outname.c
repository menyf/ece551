#include "outname.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * computeOutputFileName(const char * inputName) {
  //WRITE ME
  int len = strlen(inputName);
  char * rt = malloc((len + 8) * sizeof(*rt));
  strncpy(rt, inputName, len + 8);
  strncpy(rt + len, ".counts", 8);
  return rt;
}
