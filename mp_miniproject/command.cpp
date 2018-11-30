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

void SetCommand::exec() {
  try {
    if (get_args().size() != 2) {
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
  std::vector<std::string> & args = get_args();
  //  std::cout << " Setting $" << args[0] << " to " << args[1] << "\n";
  shell->set_variable(args[0], args[1]);
}

void ExportCommand::exec() {
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
  std::vector<std::string> & args = get_args();
  //  std::cout << " Setting $" << args[0] << " to " << args[1] << "\n";
  std::string key_copy = args[0];
  char * key = (char *)key_copy.c_str();
  std::string val_copy = shell->get_variable(key_copy);
  char * val = (char *)val_copy.c_str();
  //  std::cout << "Setting " << key << " to " << val << "\n";
  setenv(key, val, 1);
  shell->update_variable();
}

void IncCommand::exec() {
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
  std::vector<std::string> & args = get_args();
  //  std::cout << " Setting $" << args[0] << " to " << args[1] << "\n";
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
