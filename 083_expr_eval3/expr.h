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
  virtual ~NumExpression() {}
  virtual std::string toString() const {
    std::stringstream ss;
    ss << val;
    return ss.str();
  }
  virtual long evaluate() const { return val; }
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
  std::string printString(char ch) const {
    std::stringstream ss;
    ss << "(" << lhs->toString() << " " << ch << " " << rhs->toString() << ")";
    return ss.str();
  }
};

// PlusExpression =================================
class PlusExpression : public OperationExpression
{
 public:
  PlusExpression(Expression * lhs, Expression * rhs) : OperationExpression(lhs, rhs) {}
  virtual ~PlusExpression() {}
  virtual std::string toString() const { return printString('+'); }
  virtual long evaluate() const { return lhs->evaluate() + rhs->evaluate(); }
};

// MinusExpression ==============================
class MinusExpression : public OperationExpression
{
 public:
  MinusExpression(Expression * lhs, Expression * rhs) : OperationExpression(lhs, rhs) {}
  virtual ~MinusExpression() {}
  virtual std::string toString() const { return printString('-'); }
  virtual long evaluate() const { return lhs->evaluate() - rhs->evaluate(); }
};

// TimesExpression ===============================
class TimesExpression : public OperationExpression
{
 public:
  TimesExpression(Expression * lhs, Expression * rhs) : OperationExpression(lhs, rhs) {}
  virtual ~TimesExpression() {}
  virtual std::string toString() const { return printString('*'); }
  virtual long evaluate() const { return lhs->evaluate() * rhs->evaluate(); }
};

// DivExpression ================================
class DivExpression : public OperationExpression
{
 public:
  DivExpression(Expression * lhs, Expression * rhs) : OperationExpression(lhs, rhs) {}
  virtual ~DivExpression() {}
  virtual std::string toString() const { return printString('/'); }
  virtual long evaluate() const { return lhs->evaluate() / rhs->evaluate(); }
};
