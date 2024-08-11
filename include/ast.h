#ifndef AST_H
#define AST_H

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "ir_builder.h"

// Abstract Syntax Tree
namespace AST {

/**
 * Base class for all expressions within the AST
 */
class Expression {
  public:
    virtual ~Expression() = default;
    virtual llvm::Value *codegen() = 0;
};

/**
 * Expression class for numeric literals
 */
class Number : public Expression {
    double value;

  public:
    Number(double value) : value(value) {}

    inline llvm::Value *codegen() {
        return llvm::ConstantFP::get(*IRBuilder::context, llvm::APFloat(value));
    };
};

/**
 * Expression class for variables
 */
class Variable : public Expression {
    std::string name;

  public:
    Variable(const std::string &name) : name(name) {}

    inline llvm::Value *codegen() {
        // Look this variable up in the function.
        llvm::Value *value = IRBuilder::symbol_table[name];
        if (!value)
            throw std::runtime_error("Use of undeclared symbol: " + name);
        return value;
    }
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

    inline llvm::Value *codegen() {
        llvm::Value *left_gen = left->codegen();
        llvm::Value *right_gen = right->codegen();

        switch (operation) {
        case '+':
            return IRBuilder::builder->CreateFAdd(left_gen, right_gen,
                                                  "addtmp");
        case '-':
            return IRBuilder::builder->CreateFSub(left_gen, right_gen,
                                                  "subtmp");
        case '*':
            return IRBuilder::builder->CreateFMul(left_gen, right_gen,
                                                  "multmp");
        case '<':
            left_gen = IRBuilder::builder->CreateFCmpULT(left_gen, right_gen);
            // Convert bool 0/1 from FCmp to double 0.0 or 1.0
            return IRBuilder::builder->CreateUIToFP(
                left_gen, llvm::Type::getDoubleTy(*IRBuilder::context),
                "booltmp");

        default:
            std::string str(1, operation);
            throw std::runtime_error("Invalid operator: " + str);
        }
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

    inline llvm::Value *codegen() {
        llvm::Function *callee_func = IRBuilder::module->getFunction(callee);
        if (!callee_func)
            throw std::runtime_error("Function: " + callee + " does not exist");

        if (callee_func->arg_size() != args.size())
            throw std::runtime_error(
                "Incorrect number of args in function call: " + callee);

        std::vector<llvm::Value *> argv;
        for (int i = 0, argc = args.size(); i < argc; i++)
            argv.push_back(args[i]->codegen());

        return IRBuilder::builder->CreateCall(callee_func, argv, "calltmp");
    };
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

    const std::string &get_name() const { return name; }

    inline llvm::Function *codegen() {
        // Convert args to a vector of doubles
        std::vector<llvm::Type *> doubles(
            args.size(), llvm::Type::getDoubleTy(*IRBuilder::context));
        // Create llvm function
        auto *type = llvm::FunctionType::get(
            llvm::Type::getDoubleTy(*IRBuilder::context), doubles, false);

        auto *function =
            llvm::Function::Create(type, llvm::Function::ExternalLinkage, name,
                                   IRBuilder::module.get());
        // Rename the llvm function args to the prototype args
        unsigned index = 0;
        for (auto &arg : function->args())
            arg.setName(args[index++]);

        return function;
    };
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

    inline llvm::Function *codegen() {
        auto function = IRBuilder::module->getFunction(prototype->get_name());

        if (!function)
            function = prototype->codegen();

        if (!function->empty())
            throw std::runtime_error("Function already defined");

        // Create basic block and start insertion
        auto block =
            llvm::BasicBlock::Create(*IRBuilder::context, "entry", function);
        IRBuilder::builder->SetInsertPoint(block);

        // Record the arguments in the symbol table
        IRBuilder::symbol_table.clear();
        for (auto &arg : function->args())
            IRBuilder::symbol_table[std::string(arg.getName())] = &arg;

        // Build body
        try {
            llvm::Value *return_value = body->codegen();
            IRBuilder::builder->CreateRet(return_value);

            // Validate the generated code, checking for consistency.
            verifyFunction(*function);

        } catch (std::runtime_error &exception) {
            function->eraseFromParent(); // Might not be needed depending on how
                                         // error is handled
            throw exception;
        }

        return function;
    }
};
} // namespace AST

#endif // !AST_H
