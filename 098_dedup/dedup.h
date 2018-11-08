#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <fstream>

#include "functional"
#include "iostream"
#include "sstream"
#include "string"
//#include "unordered_map"
#include "list"
#include "vector"

class Dedup
{
 private:
  std::vector<std::list<std::pair<std::string, std::string> > > hash_table;
  //  std::unordered_map<std::string, std::string> dict;
  std::stringstream out;
  std::hash<std::string> str_hash;
  size_t mod;

 public:
  Dedup() {
    out << "#!/bin/bash\n\n";
    mod = 504937;
    hash_table.resize(mod, std::list<std::pair<std::string, std::string> >());
  }

  Dedup(const Dedup & rhs) :
      /*dict(rhs.dict),*/
      hash_table(rhs.hash_table),
      out(rhs.out.str()),
      str_hash(rhs.str_hash),
      mod(rhs.mod) {}

  virtual ~Dedup() {}

  Dedup & operator=(const Dedup & rhs) {
    if (this != &rhs) {
      Dedup temp(rhs);
      //      std::swap(dict, temp.dict);
      std::swap(hash_table, temp.hash_table);
      std::swap(out, temp.out);
      std::swap(str_hash, temp.str_hash);
      std::swap(mod, temp.mod);
    }
    return *this;
  }

  //entrance
  void run(const char * path) {
    //    std::cout << std::string(path) << "\n";
    if (isSymbolic(path)) {  // symlinks
      return;
    }
    else if (isDirectory(path)) {  // directory
      struct dirent * ptr;
      DIR * dir = opendir(path);
      while ((ptr = readdir(dir)) != NULL) {
        if (ptr->d_name[0] == '.')
          continue;
        std::string p = std::string(path) + "/" + ptr->d_name;
        run(p.c_str());
      }
      closedir(dir);
    }
    else {  // regular file
      remove_file(path);
    }
  }

  void remove_file(const char * p) {
    std::string content = read(p);
    size_t hashval = str_hash(content) % mod;

    std::list<std::pair<std::string, std::string> >::iterator it = hash_table[hashval].begin();
    while (it != hash_table[hashval].end()) {
      if (it->first == content) {
        out << "#Removing " << p << " (duplicate of " << it->second << ").\n";
        out << "rm " << p << "\n\n";
        return;
      }
    }
    hash_table[hashval].push_back({content, std::string(p)});
  }

  std::string read(const char * file) {
    std::ifstream ifs;
    std::stringstream res;
    std::string tmp;
    ifs.open(file, std::ifstream::in);
    while (std::getline(ifs, tmp)) {
      res << tmp;
    }
    ifs.close();
    return res.str();
  }

  void print() { std::cout << out.str(); }

  bool isSymbolic(const char * file) {
    struct stat sb;
    if (lstat(file, &sb) == -1) {
      perror("lstat failed");
      exit(EXIT_FAILURE);
    }
    return S_ISLNK(sb.st_mode);
  }

  bool isDirectory(const char * path) {
    struct stat sb;
    if (lstat(path, &sb) == -1) {
      perror("lstat failed");
      exit(EXIT_FAILURE);
    }
    return (sb.st_mode & S_IFMT) == S_IFDIR;
  }
};

/*
Corner Case:
1. two identical path
2. shall we remove empty directory?

Input: a path
Output: void

For each item in current directory, check whether current item is a file or a directory
If it is a symbolic link, skip
if it is a path, call the function to the sub-directory, done.
If it is a file, check 
1. existed the same, output comments and command
2. add the it to hashmap, do nothing.

*/
