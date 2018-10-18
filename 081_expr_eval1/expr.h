#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

// Expression ====================================
class Expression
{
 public:
  virtual std::string toString() const = 0;
  virtual long evaluate() const = 0;
  virtual ~Expression() {}
};

// NumExpression =================================
class NumExpression : public Expression
{
 private:
  long val;

 public:
  NumExpression(long num) : val(num) {}
  virtual std::string toString() const {
    std::string s;
    long tmp = val;
    while (tmp) {
      s += tmp % 10 + '0';
      tmp /= 10;
    }
    reverse(s.begin(), s.end());
    return s;
  }
  virtual long evaluate() const { return val; }
  virtual ~NumExpression() {}
};

// OperationExpression ===========================
class OperationExpression : public Expression
{
 protected:
  Expression * lhs;
  Expression * rhs;

 public:
  OperationExpression(Expression * lhs, Expression * rhs) : lhs(lhs), rhs(rhs) {}
  ~OperationExpression() {
    delete lhs;
    delete rhs;
  }
  virtual std::string toString() const = 0;
  virtual long evaluate() const = 0;
};

// PlusExpression =================================
class PlusExpression : public OperationExpression
{
 public:
  PlusExpression(Expression * lhs, Expression * rhs) : OperationExpression(lhs, rhs) {}
  virtual ~PlusExpression() {}
  virtual std::string toString() const {
    std::stringstream ss;
    ss << "(" << lhs->toString() << " + " << rhs->toString() << ")";
    return ss.str();
  }
  virtual long evaluate() const { return lhs->evaluate() + rhs->evaluate(); }
};

// MinusExpression ==============================
class MinusExpression : public OperationExpression
{
 public:
  MinusExpression(Expression * lhs, Expression * rhs) : OperationExpression(lhs, rhs) {}
  virtual ~MinusExpression() {}
  virtual std::string toString() const {
    std::stringstream ss;
    ss << "(" << lhs->toString() << " + " << rhs->toString() << ")";
    std::string rt;
    ss >> rt;
    return rt;
  }
  virtual long evaluate() const { return lhs->evaluate() + rhs->evaluate(); }
};

// TimesExpression ===============================
class TimesExpression : public OperationExpression
{
 public:
  TimesExpression(Expression * lhs, Expression * rhs) : OperationExpression(lhs, rhs) {}
  virtual ~TimesExpression() {}
  virtual std::string toString() const {
    std::stringstream ss;
    ss << "(" << lhs->toString() << " + " << rhs->toString() << ")";
    std::string rt;
    ss >> rt;
    return rt;
  }
  virtual long evaluate() const { return lhs->evaluate() + rhs->evaluate(); }
};

// DivExpression ================================
class DivExpression : public OperationExpression
{
 public:
  DivExpression(Expression * lhs, Expression * rhs) : OperationExpression(lhs, rhs) {}
  virtual ~DivExpression() {}
  virtual std::string toString() const {
    std::stringstream ss;
    ss << "(" << lhs->toString() << " + " << rhs->toString() << ")";
    std::string rt;
    ss >> rt;
    return rt;
  }
  virtual long evaluate() const { return lhs->evaluate() + rhs->evaluate(); }
};
