#include "shell.h"

#include <assert.h>
#include <unistd.h>

#include <cstring>

#include "command.h"
#include "parser.h"

extern char ** environ;
Shell::Shell() : command(NULL) {
  update_variable();
  //initialize the ECE551PATH environment variable withe the value of PATH
  variables["ECE551PATH"] = variables["PATH"];
  update_path();
  run_command("export ECE551PATH");
}

// The entry of Shell
void Shell::run() {
  std::string input;
  prompt();
  while (std::getline(std::cin, input)) {
    if (input == "exit")
      break;
    run_command(input);
    prompt();
  }
  prompt();
}

void Shell::run_command(std::string input) {
  try {
    Parser parser(input, this);
    command = parser.generate();
    parser.complete_command(command);
    command->exec();
    delete command;
  }
  catch (std::logic_error & e) {
    std::cerr << "[myShell KNOWN Issue]" << e.what() << "\n";
  }
  catch (std::exception & e) {
    std::cerr << "[myShell UNKNOWN Issue]" << e.what() << "\n";
  }
}
void Shell::update_variable() {
  for (char ** env = environ; *env; env++) {
    std::string line = *env;
    int p = line.find("=");
    std::string key = line.substr(0, p);
    std::string val = line.substr(p + 1);
    variables[key] = val;
  }
}

void Shell::update_path() {
  path.clear();

  // Initialize PATH
  std::string path_copy = variables["ECE551PATH"];
  char * _path = (char *)path_copy.c_str();
  char * pch;
  pch = strtok(_path, ":");
  while (pch != NULL) {
    path.push_back(pch);
    pch = strtok(NULL, ":");
  }
}

void Shell::prompt() {
  getcwd(current_work_directory, MAX_DIRECTORY_LENGTH);
  std::cout << "myShell:" << current_work_directory << " $";
}
