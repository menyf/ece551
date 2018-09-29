#include "election.h"

#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
//include any other headers you need here...
void parse_state_name(char * dest, const char * src, const int len) {
  for (int i = 0; i < len; i++) {
    dest[i] = src[i];
  }
  dest[len] = 0;
}

uint64_t parse_population(const char * str, int l, int r) {
  uint64_t ans = 0ULL;
  for (int i = l; i < r; i++) {
    ans = ans * 10 + (uint64_t)(str[i] - '0');
  }
  return ans;
}

unsigned parse_vote_number(const char * str, int l, int r) {
  unsigned ans = 0;
  for (int i = l; i < r; i++) {
    ans = ans * 10 + (unsigned)(str[i] - '0');
  }
  return ans;
}

state_t parseLine(const char * line) {
  //STEP 1: write me
  state_t state;
  int ed = strlen(line);
  int sep1 = -1, sep2 = -1;
  int err = 0;
  for (int i = 0; i < ed; i++) {
    if (line[i] == ':') {
      if (sep1 == -1)
        sep1 = i;
      else if (sep2 == -1)
        sep2 = i;
      else
        err = 1;
    }
  }
  if (sep1 == 0 || sep1 + 1 == sep2 || sep2 + 1 == ed || sep2 == -1 || err) {
    fprintf(stderr, "Error input in this line: '%s'.\n", line);
    exit(EXIT_FAILURE);
  }
  parse_state_name(state.name, line, sep1);
  state.population = parse_population(line, sep1 + 1, sep2);
  state.electoralVotes = parse_vote_number(line, sep2 + 1, ed);
  return state;
}

unsigned int countElectoralVotes(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 2: write me

  return 0;
}

void printRecounts(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 3: write me
}

void printLargestWin(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 4: write me
}
