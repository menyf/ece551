#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
void sortLines(std::istream & is) {
  std::string tmp;
  std::vector<std::string> str_arr;
  while (std::getline(is, tmp)) {
    str_arr.push_back(tmp);
  }

  sort(str_arr.begin(), str_arr.end());

  std::vector<std::string>::iterator it = str_arr.begin();
  while (it != str_arr.end()) {
    std::cout << *it << std::endl;
    it++;
  }
}
int main(int argc, char ** argv) {
  if (argc == 1)
    sortLines(std::cin);
  std::ifstream ifs;
  for (int i = 1; i < argc; i++) {
    ifs.open(argv[i], std::ifstream::in);
    if (!ifs) {
      std::cerr << "Can not open file." << std::endl;
      exit(EXIT_FAILURE);
    }

    sortLines(ifs);
    ifs.close();
  }

  return EXIT_SUCCESS;
}
