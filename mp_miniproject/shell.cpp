#include "shell.h"
#include "parser.h"
#include "command.h"
#include <assert.h>
#include <unistd.h>
#include <cstring>

extern char **environ;
Shell::Shell() : command(NULL)
{
  update_variable();
}

// The entry of Shell
void Shell::run()
{
  std::string input;
  prompt();
  while (std::getline(std::cin, input))
  {
    if (input == "exit")
      break;
    Parser parser(input, this);
    command = parser.generate();
    if (command != NULL)
    {
      parser.complete_command(command); 
      command->exec();
      delete command;
    }
    prompt();
  }
}

void Shell::update_variable()
{
  for (char **env = environ; *env; env++)
  {
    std::string line = *env;
    int p = line.find("=");
    std::string key = line.substr(0, p);
    std::string val = line.substr(p + 1);
    variables[key] = val;
  }
  update_path();
}

void Shell::update_path()
{
  path.clear();
  
  // Initialize PATH
  std::string path_copy = variables["PATH"];
  char *_path = (char *)path_copy.c_str();
  char *pch;
  pch = strtok(_path, ":");
  while (pch != NULL)
  {
    path.push_back(pch);
    pch = strtok(NULL, ":");
  }
}

void Shell::prompt()
{
  getcwd(current_work_directory, MAX_DIRECTORY_LENGTH);
  std::cout << "myShell:" << current_work_directory << " $";
}
