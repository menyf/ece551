#include "shell.h"

#include <cstring>
extern char ** environ;
Shell::Shell() : command(NULL) {
  update_variable();
}

void Shell::update_path() {
  path.clear();

  // Initialize PATH
  std::string path_copy = variables["PATH"];
  char * _path = (char *)path_copy.c_str();
  //  std::cout << variables["PATH"] << "\n";
  char * pch;
  pch = strtok(_path, ":");
  while (pch != NULL) {
    path.push_back(pch);
    //    std::cout << path.back() << "\n";
    pch = strtok(NULL, ":");
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
  update_path();
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
    if (command != NULL) {
      parser.complete_command(command);
      command->exec();
      delete command;
    }
    else {
      std::cerr << "Arguments error\n";
    }
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
  else if (command == "set") {
    if (parse_to_set() == 0)
      return NULL;
    SetCommand * setCommand = new SetCommand(args);
    setCommand->setShell(shell);
    return setCommand;
  }
  else if (command == "export") {
    ExportCommand * exportCommand = new ExportCommand(args);
    exportCommand->setShell(shell);
    return exportCommand;
  }
  else if (command == "inc") {
    IncCommand * incCommand = new IncCommand(args);
    incCommand->setShell(shell);
    return incCommand;
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
  //  std::cout << "replacing " << str << " to " << res << "\n";
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
  while (i < input.length() && input[i] == ' ')
    i++;
  size_t l = i;
  while (i < input.length()) {
    if (input[i] == ' ' && input[i - 1] != '\\')
      break;
    i++;
  }
  args[0] = input.substr(l, i - l);
  while (i < input.length() && input[i] == ' ')
    i++;
  args[1] = input.substr(i);
  args[1] = expand_var_in_arg(args[1]);
  return args[0] != "" && args[1] != "";
}
