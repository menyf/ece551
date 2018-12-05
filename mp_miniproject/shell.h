#ifndef SHELL_H
#define SHELL_H

#include <map>
#include <string>
#include <vector>

#define MAX_DIRECTORY_LENGTH 256

// Forward declaration
class Command;

/*
Class Name: Shell
Description: Take control of everything. 
1. When we initialize a Shell object, it would read all env variables
2. When variable is exported, update env variables
3. It reads a command each time, and run it until meets a "exit" command or C-d
*/
class Shell
{
 private:
  Command * command;  // current command to be executed.
  char current_work_directory[MAX_DIRECTORY_LENGTH];
  std::vector<std::string> path;                 // $ECE551PATH variable
  std::map<std::string, std::string> variables;  // all variables

  void run_command(std::string input);

 public:
  Shell();
  void run();              // the entry of shell
  void update_variable();  // read variables from env
  void update_path();
  void prompt();  // prompt infomation before entering command.

  // getter and setters
  void set_variable(std::string key, std::string val) { variables[key] = val; };
  std::map<std::string, std::string> & get_variable() { return variables; }
  std::string get_variable(std::string key) {
    return variables.find(key) != variables.end() ? variables[key] : "";
  }
  const std::vector<std::string> & get_path() { return path; }
};
#endif
