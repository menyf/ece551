#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

class Shell;
class Command
{
 private:
  std::string command;
  std::vector<std::string> args;

 public:
  Command() {}
  Command(std::string command, std::vector<std::string> args) : command(command), args(args) {}
  Command(const Command & rhs) : command(rhs.command), args(rhs.args) {}
  virtual ~Command() {}
  //Command & operator=(const Command & rhs);
  //Command & operator=(const std::string & rhs);
  virtual void exec();
  void exec_child_process();
  void exec_parent_process(pid_t & cpid, pid_t & w, int & wstatus);
  std::string get_command() { return command; }
  void set_command(const std::string str) { command = str; }
  std::vector<std::string> & get_args() { return args; }
};

class ChangeDirectoryCommand : public Command
{
 private:
  class ArgsException : public std::exception
  {
   public:
    const char * what() const throw() { return "Bad arguments"; }
  };

 public:
  ChangeDirectoryCommand(std::vector<std::string> args) : Command("cd", args) {}
  virtual void exec();
};

class SetCommand : public Command
{
 private:
  Shell * shell;
  class ArgsException : public std::exception
  {
   public:
    const char * what() const throw() { return "Bad arguments"; }
  };

 public:
  SetCommand(std::vector<std::string> args) : Command("set", args) {}
  virtual void exec();
  void setShell(Shell * shell) { this->shell = shell; }
};

class ExportCommand : public Command
{
 private:
  Shell * shell;
  class ArgsException : public std::exception
  {
   public:
    const char * what() const throw() { return "Bad arguments"; }
  };

 public:
  ExportCommand(std::vector<std::string> args) : Command("export", args) {}
  virtual void exec();
  void setShell(Shell * shell) { this->shell = shell; }
};

class IncCommand : public Command
{
 private:
  class ArgsException : public std::exception
  {
   public:
    const char * what() const throw() { return "Bad arguments"; }
  };
  Shell * shell;

 public:
  IncCommand(std::vector<std::string> args) : Command("inc", args) {}
  virtual void exec();
  void setShell(Shell * shell) { this->shell = shell; }
  int read_number(std::string str);
};

#endif
