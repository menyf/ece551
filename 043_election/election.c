#include "election.h"

#include "ctype.h"
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

enum error_tag {
  LEADING_COLON,
  TAILING_COLON,
  LESS_COLON,
  MORE_COLON,
  ADJACENT_COLONS,
  WRONG_POPULATION_NUMBER,
  WRONG_VOTE_NUMBER,
  TOO_LONG_INPUT,
  NO_ERROR
};

typedef enum error_tag error_t;
state_t parseLine(const char * line) {
  //STEP 1: write me
  state_t state;
  int col1 = -1, col2 = -1, ed = strlen(line);
  error_t err = ed > 63 ? TOO_LONG_INPUT : NO_ERROR;
  if (line[0] == ':') {
    err = LEADING_COLON;
  }
  else if (line[ed - 1] == ':') {
    err = TAILING_COLON;
  }
  for (int i = 0; i < ed && err == NO_ERROR; i++) {
    if (i && line[i] == ':' && line[i - 1] == ':') {
      err = ADJACENT_COLONS;
    }
    if (line[i] == ':') {
      if (col1 == -1) {
        col1 = i;
      }
      else if (col2 == -1) {
        col2 = i;
      }
      else {
        err = MORE_COLON;
      }
    }
    else {
      if (col1 != -1 && col2 == -1 && !isdigit(line[i])) {
        err = WRONG_POPULATION_NUMBER;
      }
      else if (col1 != -1 && col2 != -1 && !isdigit(line[i])) {
        err = WRONG_VOTE_NUMBER;
      }
    }
  }
  if (err == NO_ERROR && col2 == -1) {
    err = LESS_COLON;
  }
  // error handling
  if (err == LEADING_COLON) {
    fprintf(stderr, "This line has at least one leading ':'(s) : %s\n", line);
    exit(EXIT_FAILURE);
  }
  else if (err == TAILING_COLON) {
    fprintf(stderr, "This line has at least one tailing ':'(s) : %s\n", line);
    exit(EXIT_FAILURE);
  }
  else if (err == LESS_COLON) {
    fprintf(stderr, "This line does not have enough ':'s : %s\n", line);
    exit(EXIT_FAILURE);
  }
  else if (err == MORE_COLON) {
    fprintf(stderr, "This line has too many ':'s : %s\n", line);
    exit(EXIT_FAILURE);
  }
  else if (err == ADJACENT_COLONS) {
    fprintf(stderr, "This line has adjacent ':'s : %s\n", line);
    exit(EXIT_FAILURE);
  }
  else if (err == WRONG_POPULATION_NUMBER) {
    fprintf(stderr, "There is some problem with the population number : %s\n", line);
    exit(EXIT_FAILURE);
  }
  else if (err == WRONG_VOTE_NUMBER) {
    fprintf(stderr, "There is some problem with the electoral vote number : %s\n", line);
    exit(EXIT_FAILURE);
  }
  else if (err == TOO_LONG_INPUT) {
    fprintf(stderr, "Thise line is too long : %s\n", line);
    exit(EXIT_FAILURE);
  }

  // exclude all possible errors
  parse_state_name(state.name, line, col1);
  state.population = parse_population(line, col1 + 1, col2);
  state.electoralVotes = parse_vote_number(line, col2 + 1, ed);
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
