#include "parser.h"

#include <unistd.h>

#include "assert.h"
#include "command.h"
#include "shell.h"

std::string Parser::format() {
  // (a) trim leading spaces,
  // (b) continuous spaces between args
  // (c) trim tailing spaces

  std::string formatted_input;
  /* We use i to iterate the whole string
   * we use j to indicate the index of the last meaningful char
   */
  int j = -1;
  for (size_t i = 0; i < input.size(); i++) {
    if (input[i] == ' ' && j == -1) {  // leading space
      continue;
    }
    else if (input[i] == ' ' && input[i - 1] != '\\') {  // space seperator
      if ((int)i == j + 1) {
        formatted_input += input[i];
      }
    }
    else {  // meaningful char
      formatted_input += input[i];
      j = i;
    }
  }

  // tailing space
  int len_formatted_input = formatted_input.length();
  if (formatted_input[len_formatted_input - 1] == ' ' &&
      formatted_input[len_formatted_input - 2] != '\\')
    formatted_input = formatted_input.substr(0, len_formatted_input - 1);

  return formatted_input;
}

void Parser::parse() {
  std::string cmd = input;
  std::vector<std::string> strs;

  // Step1: Generate a formatted string
  std::string formatted_cmd = format();

  // Step2: divide string according to space seperator
  size_t l = 0;  // the left most index of current arg, note a command is also a arg
  for (size_t i = 0; i < formatted_cmd.length(); i++) {
    if (formatted_cmd[i] == ' ' && formatted_cmd[i - 1] != '\\')  // a space seperator hit
    {
      std::string sub_str = formatted_cmd.substr(l, i - l);
      // parse all escaped blank to be blank
      std::string parsed_blank_str = parse_blank(sub_str);
      // replace $-leading varible with real value
      std::string expanded_var_str = expand_var_in_arg(parsed_blank_str);
      // collect this arg
      strs.push_back(expanded_var_str);
      l = i + 1;  // move to the start position of next arg
    }
  }
  std::string sub_str = formatted_cmd.substr(l);
  std::string parsed_blank_str = parse_blank(sub_str);
  std::string expanded_var_str = expand_var_in_arg(parsed_blank_str);
  strs.push_back(expanded_var_str);

  // Step3: extract values
  if (strs.empty() || strs[0].empty()) {
    throw std::logic_error("You entered an empty command.");
  }
  command = strs[0];
  strs.erase(strs.begin());
  args = strs;
}

std::string Parser::parse_blank(std::string str) {
  std::string res;
  for (size_t i = 0; i < str.length(); i++) {
    if (str[i] == '\\' && i + 1 < str.length() && str[i + 1] == ' ') {
      res += ' ';
      i++;
    }
    else {
      res += str[i];
    }
  }
  return res;
}

Command * Parser::generate() {
  if (command == "cd") {
    ChangeDirectoryCommand * changeDirectoryCommand = new ChangeDirectoryCommand(args);
    set_redirection(changeDirectoryCommand);
    return changeDirectoryCommand;
  }
  else if (command == "set") {
    if (parse_to_set() == 0)
      throw std::logic_error("The arguments seems bad, please follow 'set var value' format");
    SetCommand * setCommand = new SetCommand(args);
    setCommand->setShell(shell);
    return setCommand;
  }
  else if (command == "export") {
    if (args.size() < (size_t)1) {
      throw std::logic_error("There should be at lease an argument to be exported");
    }
    else if (args.size() > (size_t)1) {
      throw std::logic_error("You can only export one argument at a time");
    }
    ExportCommand * exportCommand = new ExportCommand(args);
    exportCommand->setShell(shell);
    return exportCommand;
  }
  else if (command == "inc") {
    if (args.size() < (size_t)1) {
      throw std::logic_error("There should be at lease an argument to be 'inc'ed");
    }
    else if (args.size() > (size_t)1) {
      throw std::logic_error("You can only inc one argument at a time");
    }
    IncCommand * incCommand = new IncCommand(args);
    incCommand->setShell(shell);
    return incCommand;
  }
  Command * command = new Command(this->command, args);
  set_redirection(command);
  return command;
}

void Parser::complete_command(Command * _cmd) {
  std::string _command = _cmd->get_command();
  if (_command.find("/") == std::string::npos) {
    const std::vector<std::string> path = shell->get_path();
    std::vector<std::string>::const_iterator it = path.begin();
    while (it != path.end()) {
      std::string full_path = *it + "/" + _command;
      if (access(full_path.c_str(), F_OK) != -1) {
        _cmd->set_command(full_path);
        return;
      }
      it++;
    }
  }
}

std::string Parser::expand_var_in_arg(std::string str) {
  std::string res;
  std::map<std::string, std::string> & mp = shell->get_variable();
  for (size_t i = 0; i < str.length(); i++) {
    if (str[i] == '$') {  // A variable found
      size_t l = i + 1;
      while (i + 1 < str.length() && (isalnum(str[i + 1]) || str[i + 1] == '_')) {
        i++;
      }
      res += mp[str.substr(l, i - l + 1)];
    }
    else {
      res += str[i];
    }
  }
  return res;
}

int Parser::parse_to_set() {
  args.resize(2);
  size_t i = 0;
  for (; i + 2 < input.length(); i++) {
    if (input.substr(i, 3) == "set") {
      i += 3;
      break;
    }
  }

  // skip blank
  while (i < input.length() && input[i] == ' ')
    i++;

  // parse the first arg of set command, i.e. key
  size_t l = i;  // the start position
  while (i < input.length()) {
    if (input[i] == ' ' && input[i - 1] != '\\')
      break;
    i++;
  }
  args[0] = input.substr(l, i - l);
  args[0] = expand_var_in_arg(args[0]);

  // skip blank
  while (i < input.length() && input[i] == ' ')
    i++;

  // parse the second arg of set command, i.e. val
  // note: here we take all regular blank(' ') as blank, but not only escaped
  // blank as blank
  args[1] = input.substr(i);
  args[1] = expand_var_in_arg(args[1]);

  // return value indicate whether both of the key and value successfully
  // obtained.
  return args[0] != "" && args[1] != "";
}

void Parser::set_redirection(Command * command) {
  for (size_t i = 0; i < args.size(); i++) {
    if (args[i] != "<" && args[i] != ">" && args[i] != "2>")
      continue;
    assert(i + 1 < args.size());
    if (args[i] == "<") {  // stdin
      command->set_stream(0, args[i + 1]);
    }
    else if (args[i] == ">") {  // stdout
      command->set_stream(1, args[i + 1]);
    }
    else if (args[i] == "2>") {  // stderr
      command->set_stream(2, args[i + 1]);
    }
    args.erase(args.begin() + i, args.begin() + i + 2);
    i--;
  }
  command->get_args() = this->args;
}
