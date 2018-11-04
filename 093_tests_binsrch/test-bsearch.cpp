#include "cmath"
#include "function.h"
#include "iostream"

int binarySearchForZero(Function<int, int> * f, int low, int high);

class SinFunction : public Function<int, int>
{
 public:
  virtual int invoke(int arg) { return 10000000 * (sin(arg / 100000.0) - 0.5); }
};

class BinFunction : public Function<int, int>
{
 public:
  virtual int invoke(int arg) { return arg; }
};

class CountedIntFn : public Function<int, int>
{
 protected:
  unsigned remaining;
  Function<int, int> * f;
  const char * mesg;

 public:
  CountedIntFn(unsigned n, Function<int, int> * fn, const char * m) :
      remaining(n),
      f(fn),
      mesg(m) {}
  virtual int invoke(int arg) {
    if (remaining == 0) {
      fprintf(stderr, "Too many function invocations in %s\n", mesg);
      exit(EXIT_FAILURE);
    }
    remaining--;
    return f->invoke(arg);
  }
};

void check(Function<int, int> * f, int low, int high, int expected_ans, const char * mesg);

int main() {
  SinFunction * f = new SinFunction();
  BinFunction * bf = new BinFunction();
  check(f, 0, 150000, 52359, "[MYF]: 150000 wrong");
  check(bf, -1, 4, 0, "test 1");
  check(bf, -9999, -1, -2, "test 2");

  return EXIT_SUCCESS;
}

void check(Function<int, int> * f, int low, int high, int expected_ans, const char * mesg) {
  unsigned mx_count = (low >= high) ? 1 : (log2(high - low) + 1);
  Function<int, int> * test = new CountedIntFn(mx_count, f, mesg);
  int ans = binarySearchForZero(test, low, high);
  if (ans != expected_ans) {
    std::cerr << mesg << std::endl;
    exit(EXIT_FAILURE);
  }
  delete test;
}
