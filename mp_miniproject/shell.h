#include <iostream>
#include <string>

#include "command.h"

class Shell
{
 private:
  //vector<command> commands;
  Command command;

 public:
  Shell() {}
  void run();
};

void Shell::run() {
  std::string binary_filename;
  std::cout << "myShell$ ";
  while (std::getline(std::cin, binary_filename)) {
    if (binary_filename == "exit") {
      break;
    }
    command = binary_filename;
    command.exec();
    std::cout << "myShell$ ";
  }
}
