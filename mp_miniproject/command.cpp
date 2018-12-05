#include "command.h"

#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"
#include "sstream"
#include "string"

extern char ** environ;
void Command::exec() {
  pid_t cpid, w;
  int wstatus;

  cpid = fork();

  // Failed to Fork
  if (cpid == -1) {
    perror("fork error");
    exit(EXIT_FAILURE);
  }
  else if (cpid == 0) { /* Code executed by child */
    exec_child_process();
  }
  else { /* Code executed by parent */
    exec_parent_process(cpid, w, wstatus);
  }
}

void Command::exec_child_process() {
  if (access(command.c_str(), F_OK) == -1) {  // cannot find it.
    std::cout << "Command " << command << " not found\n";
  }
  else {
    redirect_stream();
    char ** newargv = new char *[args.size() + 2];
    newargv[0] = (char *)command.c_str();
    for (size_t i = 0; i < args.size(); i++) {
      newargv[i + 1] = (char *)args[i].c_str();
    }
    newargv[args.size() + 1] = NULL;

    execve(newargv[0], newargv, environ);
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

void Command::redirect_stream() {
  if (!stream[0].empty()) {  // stdin must differ from stdout and stderr
    std::cout << "[MYF]: redirecting stdin from " << stream[0] << "\n";
    assert(stream[0] != stream[1] && stream[0] != stream[2]);
    int fd = open(stream[0].c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    dup2(fd, 0);
    close(fd);
  }

  if (stream[1] == stream[2] && !stream[1].empty()) {
    int fd = open(stream[1].c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);
  }
  else {
    for (int i = 1; i < 3; i++) {
      if (stream[i].empty())
        continue;
      int fd = open(stream[i].c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
      dup2(fd, i);
      close(fd);
    }
  }
}

void ChangeDirectoryCommand::exec() {
  std::string & arg = get_args()[0];
  if (arg[0] == '~') {
    char * _path = getenv("HOME");
    arg = _path + arg.substr(1);
  }
  redirect_stream();
  if (chdir(arg.c_str()) == -1) {
    perror("Can not change directory");
  }
}

void SetCommand::exec() {
  std::vector<std::string> & args = get_args();
  shell->set_variable(args[0], args[1]);
}

void ExportCommand::exec() {
  std::string key_copy = get_args()[0];
  char * key = (char *)key_copy.c_str();
  std::string val_copy = shell->get_variable(key_copy);
  char * val = (char *)val_copy.c_str();
  setenv(key, val, 1);
  shell->update_variable();

  // Drew: the user changes ECE551PATH, it should affect where your shell
  if (key_copy == "ECE551PATH")
    shell->update_path();
}

void IncCommand::exec() {
  std::vector<std::string> & args = get_args();
  std::string key_copy = args[0];
  std::string val_copy = shell->get_variable(key_copy);

  if (val_copy.empty()) {  // not exist
    shell->set_variable(key_copy, "1");
    return;
  }

  int num = read_number(val_copy);
  std::string val_new = "1";
  if (num != 0) {
    std::stringstream ss;
    num++;
    ss << num;
    val_new = ss.str();
  }
  shell->set_variable(key_copy, val_new);
}

int IncCommand::read_number(std::string str) {
  int res = 0;
  bool isNegtive = false;
  if (str[0] == '-') {
    str = str.substr(1);
    isNegtive = true;
  }
  if (str.empty())
    return 0;  // a single '-'
  for (size_t i = 0; i < str.length(); i++) {
    if (!isdigit(str[i]))
      return 0;
    res = res * 10 + str[i] - '0';
  }
  return isNegtive ? -res : res;
}
