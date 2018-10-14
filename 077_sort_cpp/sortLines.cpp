#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
int main(int argc, char ** argv) {
  if (argc > 2) {
    std::cerr << "Wrong number of inputs" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string tmp;
  std::vector<std::string> str_arr;
  std::ifstream ifs;

  if (argc == 2) {
    ifs.open(argv[1], std::ifstream::in);
    if (!ifs) {
      std::cerr << "Can not open file." << std::endl;
    }
  }

  std::istream & is = argc == 2 ? ifs : std::cin;

  while (std::getline(is, tmp)) {
    str_arr.push_back(tmp);
  }

  if (argc == 2) {
    ifs.close();
  }

  sort(str_arr.begin(), str_arr.end());

  std::vector<std::string>::iterator it = str_arr.begin();
  while (it != str_arr.end()) {
    std::cout << *it << std::endl;
    it++;
  }
  return EXIT_SUCCESS;
}
