#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "command.h"

class Shell
{
 private:
  //vector<Command> commands;
  Command command;
  std::vector<std::string> path;

 public:
  Shell();
  void run();
  void complete_command(std::string _command);
};

Shell::Shell() {
  // Initialize PATH
  char * _path = getenv("PATH");
  char * pch;
  pch = strtok(_path, ":");
  while (pch != NULL) {
    path.push_back(pch);
    pch = strtok(NULL, ":");
  }
}

// The entry of Shell
void Shell::run() {
  std::string binary_filename;
  std::cout << "myShell$ ";
  while (std::getline(std::cin, binary_filename)) {
    if (binary_filename == "exit") {
      break;
    }
    command = binary_filename;
    complete_command(command.get_command());
    command.exec();
    std::cout << "myShell$ ";
  }
}

void Shell::complete_command(std::string _command) {
  if (_command.find("/") == std::string::npos) {
    for (size_t i = 0; i < path.size(); i++) {
      std::string full_path = path[i] + "/" + _command;
      if (access(full_path.c_str(), F_OK) != -1) {
        command.set_command(full_path);
        return;
      }
    }
  }
}
