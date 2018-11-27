#include "shell.h"

#include <cstring>

Shell::Shell() : command(NULL) {
  // Initialize PATH
  char * _path = getenv("PATH");
  char * pch;
  pch = strtok(_path, ":");
  while (pch != NULL) {
    path.push_back(pch);
    //    std::cout << path.back() << "\n";
    pch = strtok(NULL, ":");
  }
}

// The entry of Shell
void Shell::run() {
  std::string input;
  prompt();
  while (std::getline(std::cin, input)) {
    if (input == "exit")
      break;
    Parser parser(input, this);
    command = parser.generate();
    parser.complete_command(command);
    command->exec();
    delete command;
    prompt();
  }
}

void Shell::prompt() {
  getcwd(current_work_directory, MAX_DIRECTORY_LENGTH);
  std::cout << "myShell:" << current_work_directory << " $";
}

void Parser::parse(std::string cmd) {
  std::vector<std::string> strs;
  std::string formatted_cmd;

  // Step1:
  // (a) trim leading spaces,
  // (b) continuous spaces between args
  // (c) trim tailing spaces

  /* We use i to iterate the whole string
   * we use j to indicate the index of the last meaningful char
   */
  int j = -1;
  for (size_t i = 0; i < cmd.size(); i++) {
    if (cmd[i] == ' ' && j == -1) {  // leading space
      continue;
    }
    else if (cmd[i] == ' ' && cmd[i - 1] != '\\') {
      if ((int)i == j + 1) {
        formatted_cmd += cmd[i];
      }
    }
    else {
      formatted_cmd += cmd[i];
      j = i;
    }
  }
  int len_formatted_cmd = formatted_cmd.length();
  // tailing space
  if (formatted_cmd[len_formatted_cmd - 1] == ' ' && formatted_cmd[len_formatted_cmd - 2] != '\\')
    formatted_cmd = formatted_cmd.substr(0, len_formatted_cmd - 1);

  //  std::cout << "After the first trim: '" << formatted_cmd << "'\n";

  // Step2: divide string according to space
  size_t l = 0;
  for (size_t i = 0; i < formatted_cmd.length(); i++) {
    if (formatted_cmd[i] == ' ' && formatted_cmd[i - 1] != '\\') {
      std::string sub_str = formatted_cmd.substr(l, i - l);
      std::string parsed_blank_str = parse_blank(sub_str);
      std::string expanded_var_str = expand_var_in_arg(parsed_blank_str);
      strs.push_back(expanded_var_str);
      l = i + 1;
    }
  }
  std::string sub_str = formatted_cmd.substr(l);
  std::string parsed_blank_str = parse_blank(sub_str);
  std::string expanded_var_str = expand_var_in_arg(parsed_blank_str);
  strs.push_back(expanded_var_str);

  // Step3: extract values
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
    return new ChangeDirectoryCommand(args);
  }
  return new Command(command, args);
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
  return str;
}
