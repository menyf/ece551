#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "iostream"
#include "string"

void run_bin_file(std::string binary_filename) {
  pid_t cpid, w;
  int wstatus;

  cpid = fork();
  if (cpid == -1) {
    perror("fork error");
    exit(EXIT_FAILURE);
  }

  if (cpid == 0) { /* Code executed by child */
    char * newargv[] = {NULL, NULL};
    char * newenviron[] = {NULL};
    execve(binary_filename.c_str(), newargv, newenviron);
    perror("execve"); /* execve() returns only on error */
  }
  else { /* Code executed by parent */
    w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
    if (w == -1) {
      perror("Waitpid Error");
      exit(EXIT_FAILURE);
    }

    if (WIFEXITED(wstatus)) {
      std::cout << "Program exited with status " << WEXITSTATUS(wstatus) << "\n";
    }
    else if (WIFSIGNALED(wstatus)) {
      std::cout << "Program was killed by signal " << WTERMSIG(wstatus) << "\n";
    }
    else {
      perror("Oops...Something goes wrong");
      exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char ** argv) {
  std::string binary_filename;
  std::cout << "myShell$ ";
  while (std::getline(std::cin, binary_filename)) {
    if (binary_filename == "exit") {
      break;
    }
    run_bin_file(binary_filename);
    std::cout << "myShell$ ";
  }
  return EXIT_SUCCESS;
}
