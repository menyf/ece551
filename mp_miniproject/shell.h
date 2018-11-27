#ifndef SHELL_H
#define SHELL_H

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
  void set_variable(std::string key, std::string val) { variables[key] = val; };
  std::map<std::string, std::string> & get_variable() { return variables; }
  const std::vector<std::string> & get_path() { return path; }
  void prompt();
};

class Parser
{
 private:
  Shell * shell;
  std::string command;
  std::vector<std::string> args;

 public:
  Parser(std::string str = "", Shell * shell = NULL) : shell(shell) { parse(str); }
  Command * generate();
  void parse(std::string cmd);
  std::string parse_blank(std::string str);
  void complete_command(Command * _cmd);
  std::string expand_var_in_arg(std::string str);
};

#endif
