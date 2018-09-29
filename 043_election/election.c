#include "election.h"

#include "assert.h"
#include "ctype.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
//include any other headers you need here...

enum error_tag {
  //  LEADING_COLON,
  TAILING_COLON,
  LESS_COLON,
  MORE_COLON,
  ADJACENT_COLONS,
  WRONG_POPULATION_NUMBER,
  WRONG_VOTE_NUMBER,
  TOO_LONG_INPUT,
  NEW_LINE_CONTAINED,
  NO_ERROR
};

typedef enum error_tag error_t;

void handle_error(error_t err, const char * line) {
  /* if (err == LEADING_COLON) { */
  /*   fprintf(stderr, "This line has at least one leading ':'(s) : %s\n", line); */
  /*   exit(EXIT_FAILURE); */
  /* } */
  /* else */
  if (err == TAILING_COLON) {
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
    fprintf(stderr, "This line is too long : %s\n", line);
    exit(EXIT_FAILURE);
  }
  else if (err == NEW_LINE_CONTAINED) {
    fprintf(stderr, "This input include '\n' character: %s\n", line);
    exit(EXIT_FAILURE);
  }
}

int compare_two_numbers(const char * num, int len_num, const char * maxValue) {
  int len_max = strlen(maxValue);
  if (len_num < len_max)
    return 1;
  if (len_num > len_max)
    return 0;
  for (int i = 0; i < len_num; i++) {
    if (num[i] > maxValue[i]) {
      return 0;
    }
    else if (num[i] < maxValue[i])
      return 1;
  }
  return 1;  // equal
}

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
  int col1 = -1, col2 = -1, ed = strlen(line);
  error_t err = NO_ERROR;
  /* if (line[0] == ':') { */
  /*   err = LEADING_COLON; */
  /* } */
  /* else  */
  if (line[ed - 1] == ':') {
    err = TAILING_COLON;
  }
  else if (strchr(line, '\n') != NULL) {
    err = NEW_LINE_CONTAINED;
  }
  for (int i = 0; i < ed && err == NO_ERROR; i++) {
    if (i && line[i] == ':' && line[i - 1] == ':') {
      err = ADJACENT_COLONS;
    }
    if (line[i] == ':') {
      if (col1 == -1) {
        col1 = i;
        if (col1 > MAX_STATE_NAME_LENGTH - 1) {
          err = TOO_LONG_INPUT;
        }
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
  if (err == NO_ERROR) {
    if (col2 == -1) {  // no or one ':' only.
      err = LESS_COLON;
    }
    else if (!compare_two_numbers(line + col1 + 1, col2 - (col1 + 1), "18446744073709551615")) {
      err = WRONG_POPULATION_NUMBER;
    }
    else if (!compare_two_numbers(line + col2 + 1, ed - (col2 + 1), "4294967295")) {
      err = WRONG_VOTE_NUMBER;
    }
  }

  // error handling
  handle_error(err, line);

  // Excluded all possible errors
  parse_state_name(state.name, line, col1);
  state.population = parse_population(line, col1 + 1, col2);
  state.electoralVotes = parse_vote_number(line, col2 + 1, ed);
  return state;
}

unsigned int countElectoralVotes(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 2: write me
  unsigned int ans = 0;
  for (size_t i = 0; i < nStates; i++) {
    if (voteCounts[i] > stateData[i].population / 2) {
      ans += stateData[i].electoralVotes;
    }
  }
  return ans;
}

void printRecounts(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 3: write me
  for (size_t i = 0; i < nStates; i++) {
    uint64_t lower_bound = (long double)stateData[i].population * 0.495 + 0.5;
    uint64_t upper_bound = (long double)stateData[i].population * 0.505 + 0.5;
    if (lower_bound <= voteCounts[i] && voteCounts[i] <= upper_bound) {
      printf("%s requires a recount (Candidate A has %.2f%% of the vote)\n",
             stateData[i].name,
             100.0f * voteCounts[i] / stateData[i].population);
    }
  }
}

void printLargestWin(state_t * stateData, uint64_t * voteCounts, size_t nStates) {
  //STEP 4: write me
  double percentage = 0;
  int idx = -1;
  for (size_t i = 0; i < nStates; i++) {
    long double curr_percentage = (long double)voteCounts[i] / stateData[i].population;
    if (curr_percentage > percentage) {
      percentage = curr_percentage;
      idx = (int)i;
    }
  }
  printf("Candidate A won %s with %.2f%% of the vote\n", stateData[idx].name, percentage * 100.0);
}
