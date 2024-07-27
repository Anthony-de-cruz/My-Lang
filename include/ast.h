#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>

// Abstract Syntax Tree
namespace AST {

/**
 * Base class for all expressions within the AST
 */
class Expression {
  public:
    virtual ~Expression() = default;
};

/**
 * Expression class for numeric literals
 */
class Number : public Expression {
    double value;

  public:
    Number(double value) : value(value) {}
};

/**
 * Expression class for variables
 */
class Variable : public Expression {
    std::string name;

  public:
    Variable(const std::string &name) : name(name) {}
};

/**
 * Expression class for binary operators
 */
class Binary : public Expression {
    char operation;
    std::unique_ptr<Expression> left, right;

  public:
    Binary(char operation, std::unique_ptr<Expression> left,
           std::unique_ptr<Expression> right)
        : operation(operation), left(std::move(left)), right(std::move(right)) {
    }
};

/**
 * Expression class for function calls
 */
class Call : public Expression {
    std::string callee;
    std::vector<std::unique_ptr<Expression>> args;

  public:
    Call(const std::string &callee,
         std::vector<std::unique_ptr<Expression>> args)
        : callee(callee), args(std::move(args)) {}
};

/**
 * Class to represent function prototypes,
 * which contains it's name and argument names
 */
class Prototype {
    std::string name;
    std::vector<std::string> args;

  public:
    Prototype(const std::string &name, std::vector<std::string> args)
        : name(name), args(std::move(args)) {}

    const std::string &getname() const { return name; }
};

/**
 * Class to represent function definitions
 */
class Function {
    std::unique_ptr<Prototype> prototype;
    std::unique_ptr<Expression> body;

  public:
    Function(std::unique_ptr<Prototype> prototype,
             std::unique_ptr<Expression> body)
        : prototype(std::move(prototype)), body(std::move(body)) {}
};
} // namespace AST

#endif // !AST_H
