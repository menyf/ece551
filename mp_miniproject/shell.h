#ifndef SHELL_H
#define SHELL_H

#include <assert.h>
#include <unistd.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "command.h"
#define MAX_DIRECTORY_LENGTH 256

class Shell
{
 private:
  Command * command;
  char current_work_directory[MAX_DIRECTORY_LENGTH];
  std::vector<std::string> path;
  std::map<std::string, std::string> variables;

 public:
  Shell();
  void run();
  void update_variable();
  void update_path();
  void set_variable(std::string key, std::string val) { variables[key] = val; };
  std::map<std::string, std::string> & get_variable() { return variables; }
  std::string get_variable(std::string key) {
    return variables.find(key) != variables.end() ? variables[key] : "";
  }
  const std::vector<std::string> & get_path() { return path; }
  void prompt();
};

class Parser
{
 private:
  Shell * shell;
  std::string input;
  std::string command;
  std::vector<std::string> args;

  std::string parse_blank(std::string str);
  std::string expand_var_in_arg(std::string str);
  int parse_to_set();

 public:
  Parser(std::string str = "", Shell * shell = NULL) : shell(shell), input(str) { parse(str); }
  void complete_command(Command * _cmd);
  void parse(std::string cmd);
  Command * generate();
};

#endif
