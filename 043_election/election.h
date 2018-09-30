#ifndef ELECTION_H
#define ELECTION_H
#include <stdint.h>
#include <stdlib.h>
#define MAX_STATE_NAME_LENGTH 64

struct state_tag {
  char name[MAX_STATE_NAME_LENGTH];
  uint64_t population;
  unsigned int electoralVotes;
};
typedef struct state_tag state_t;

state_t parseLine(const char * line);

unsigned int countElectoralVotes(state_t * stateData, uint64_t * voteCounts, size_t nStates);

void printRecounts(state_t * stateData, uint64_t * voteCounts, size_t nStates);

void printLargestWin(state_t * stateData, uint64_t * voteCounts, size_t nStates);

// Customized below

enum error_tag {
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

void handle_error(error_t err, const char * line);
int compare_two_numbers(const char * num, int len_num, const char * maxValue);
error_t find_error(const char * line, int * col1, int * col2, int * ed);
unsigned parse_vote_number(const char * str, int l, int r);
uint64_t parse_population(const char * str, int l, int r);
void parse_state_name(char * dest, const char * src, const int len);

#endif
