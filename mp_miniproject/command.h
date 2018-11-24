#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
class Command
{
 private:
  std::string command;

 public:
  Command() {}
  Command(std::string command) : command(command) {}
  Command(const Command & rhs) : command(rhs.command) {}
  virtual ~Command() {}
  Command & operator=(const Command & rhs);
  Command & operator=(const std::string & rhs);
  void exec();
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

  if (cpid == 0) { /* Code executed by child */
    char * newargv[] = {NULL};
    char * newenviron[] = {NULL};
    execve(command.c_str(), newargv, newenviron);
    perror("execve"); /* execve() returns only on error */
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
  }
  return *this;
}

Command & Command::operator=(const std::string & rhs) {
  Command temp(rhs);
  (*this) = temp;
  return *this;
}
