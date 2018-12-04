#ifndef COMMAND_H
#define COMMAND_H



#include <algorithm>
#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

class Shell;

/*
Class Name: Command
Description: A command with arguments. It can be a regular command, which is itself. For this case, we will call fork and execve to run this command. For special command that I implement it myself, we will call some library function. These commands would be regared as derived classes.
*/
class Command
{
private:
  std::string command;
  std::vector<std::string> args;

protected:
  Shell *shell;

public:
  Command() : shell(NULL) {}
  Command(std::string command, std::vector<std::string> args) : command(command), args(args) {}
  virtual ~Command() {}

  // execute function.
  // regular command would call fork and execve
  // derived commands would call libraries functions
  virtual void exec();
  void exec_child_process();
  void exec_parent_process(pid_t &cpid, pid_t &w, int &wstatus);

  // getters and setters
  std::string get_command() { return command; }
  void set_command(const std::string str) { command = str; }
  std::vector<std::string> &get_args() { return args; }
  void setShell(Shell *shell) { this->shell = shell; }
};


class ChangeDirectoryCommand : public Command
{

public:
  ChangeDirectoryCommand(std::vector<std::string> args) : Command("cd", args) {}
  virtual void exec();
};

class SetCommand : public Command
{

public:
  SetCommand(std::vector<std::string> args) : Command("set", args) {}
  virtual void exec();
};

class ExportCommand : public Command
{
public:
  ExportCommand(std::vector<std::string> args) : Command("export", args) {}
  virtual void exec();
};

class IncCommand : public Command
{
public:
  IncCommand(std::vector<std::string> args) : Command("inc", args) {}
  virtual void exec();
  int read_number(std::string str);
};

#endif
