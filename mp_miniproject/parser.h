#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>

// Forward declarations
class Shell;
class Command;

/*
Class Name: Parser
Description: Doing edition to a string, make it a valid command-and-args pair.
*/
class Parser
{
 private:
  Shell * shell;  // current shell
  const std::string input;
  std::string command;
  std::vector<std::string> args;

  // replace all escaped blank(i.e. '\ ') to blank(i.e. ' ')
  std::string parse_blank(std::string str);

  // replace all $-leading varible with real value
  std::string expand_var_in_arg(std::string str);

  // parse current input to a set command.
  int parse_to_set();  // special case, args be parsed in different way.

  std::string format();

 public:
  Parser(std::string str = "", Shell * shell = NULL) : shell(shell), input(str) { parse(); }

  // complete command to a full path command according to PATH in shell
  void complete_command(Command * _cmd);

  // parse input to regular command
  void parse();

  // generate command according to input
  Command * generate();

  // set rediections to the command.
  void set_redirection(Command * command);

  bool isValidVariable(std::string);
};
#endif
