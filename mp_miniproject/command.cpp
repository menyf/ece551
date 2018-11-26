#include "command.h"

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
    exec_child_process();
  }
  else { /* Code executed by parent */
    exec_parent_process(cpid, w, wstatus);
  }
}

/*Command & Command::operator=(const Command & rhs) {
  if (this != &rhs) {
    Command temp(rhs);
    std::swap(temp.command, command);
    std::swap(temp.args, args);
  }
  return *this;
  }*/

void Command::exec_child_process() {
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

void Command::exec_parent_process(pid_t & cpid, pid_t & w, int & wstatus) {
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

void ChangeDirectoryCommand::exec() {
  try {
    if (get_args().size() != 1) {
      throw ArgsException();
      return;
    }
  }
  catch (ArgsException & e) {
    std::cerr << e.what() << std::endl;
  }
  catch (std::exception & e) {
    std::cerr << e.what() << std::endl;
  }

  std::string & arg = get_args()[0];
  std::cout << "changing to:" << arg << "\n";
  if (arg[0] == '~') {
    char * _path = getenv("HOME");
    arg = _path + arg.substr(1);
  }
  if (chdir(arg.c_str()) == -1) {
    perror("Can not change directory");
  }
}
