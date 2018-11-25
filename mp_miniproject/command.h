#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

class Command
{
 private:
  std::string command;
  std::vector<std::string> args;

 public:
  Command() {}
  Command(std::string command) { parse(command); }
  Command(const Command & rhs) : command(rhs.command), args(rhs.args) {}
  virtual ~Command() {}
  Command & operator=(const Command & rhs);
  Command & operator=(const std::string & rhs);
  void exec();
  void parse(std::string cmd);
  std::string parse_blank(std::string str);
  std::string get_command() { return command; }
  void set_command(const std::string str) { command = str; }
};

void Command::exec() {
  pid_t cpid, w;
  int wstatus;

  cpid = fork();

  // Failed to Fork
  if (cpid == -1) {
    perror("fork error");
    exit(EXIT_FAILURE);
  }

  if (cpid == 0) {                              /* Code executed by child */
    if (access(command.c_str(), F_OK) == -1) {  // cannot find it.
      std::cout << "Command " << command << " not found\n";
    }
    else {
      char ** newargv = new char *[args.size() + 2];
      newargv[0] = (char *)command.c_str();
      for (size_t i = 0; i < args.size(); i++) {
        newargv[i + 1] = (char *)args[i].c_str();
      }
      newargv[args.size() + 1] = NULL;

      char * newenviron[] = {NULL};
      execve(newargv[0], newargv, newenviron);
      perror("execve"); /* execve() returns only on error */
      delete[] newargv;
    }
  }
  else { /* Code executed by parent */
    w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);

    // Failed to obtain info of child process
    if (w == -1) {
      perror("Waitpid Error");
      exit(EXIT_FAILURE);
    }

    if (WIFEXITED(wstatus)) {  // Successfully exit
      std::cout << "Program exited with status " << WEXITSTATUS(wstatus) << "\n";
    }
    else if (WIFSIGNALED(wstatus)) {  // Killed
      std::cout << "Program was killed by signal " << WTERMSIG(wstatus) << "\n";
    }
    else {  // Other situations
      perror("Oops...Something goes wrong");
      exit(EXIT_FAILURE);
    }
  }
}

Command & Command::operator=(const Command & rhs) {
  if (this != &rhs) {
    Command temp(rhs);
    std::swap(temp.command, command);
    std::swap(temp.args, args);
  }
  return *this;
}

Command & Command::operator=(const std::string & rhs) {
  parse(rhs);
  return *this;
}

void Command::parse(std::string cmd) {
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
      strs.push_back(parse_blank(sub_str));
      l = i + 1;
    }
  }
  strs.push_back(parse_blank(formatted_cmd.substr(l)));

  // Step3: extract values
  command = strs[0];
  strs.erase(strs.begin());
  args = strs;
}

std::string Command::parse_blank(std::string str) {
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
