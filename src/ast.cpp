
#include <iostream>
#include <stdexcept>

#include <llvm/IR/IRBuilder.h>

#include "ast.h"
#include "llvm_components.h"

namespace AST {

llvm::Value *Number::codegen() {
    std::cout << "store numeric " << this->value << '\n';

    return llvm::ConstantFP::get(*LLVMComponents::context,
                                 llvm::APFloat(this->value));
};

llvm::Value *Variable::codegen() {
    // Look this variable up in the function.
    llvm::Value *value = LLVMComponents::symbol_table[this->name];
    if (!value)
        throw std::runtime_error("Use of undeclared symbol: " + this->name);
    return value;
}

llvm::Value *Binary::codegen() {
    llvm::Value *left_gen = left->codegen();
    llvm::Value *right_gen = right->codegen();

    switch (operation) {
    case '+':
        return LLVMComponents::builder->CreateFAdd(left_gen, right_gen,
                                                   "addtmp");
    case '-':
        return LLVMComponents::builder->CreateFSub(left_gen, right_gen,
                                                   "subtmp");
    case '*':
        return LLVMComponents::builder->CreateFMul(left_gen, right_gen,
                                                   "multmp");
    case '<':
        left_gen = LLVMComponents::builder->CreateFCmpULT(left_gen, right_gen);
        // Convert bool 0/1 from FCmp to double 0.0 or 1.0
        return LLVMComponents::builder->CreateUIToFP(
            left_gen, llvm::Type::getDoubleTy(*LLVMComponents::context),
            "booltmp");

    default:
        std::string str(1, operation);
        throw std::runtime_error("Invalid operator: " + str);
    }
}

llvm::Value *Call::codegen() {
    llvm::Function *callee_func = LLVMComponents::module->getFunction(callee);
    if (!callee_func)
        throw std::runtime_error("Function: " + this->callee +
                                 " does not exist");

    if (callee_func->arg_size() != this->args.size())
        throw std::runtime_error("Incorrect number of args in function call: " +
                                 this->callee);

    std::vector<llvm::Value *> argv;
    argv.reserve(this->args.size());
    for (const auto &arg : this->args)
        argv.push_back(arg->codegen());

    return LLVMComponents::builder->CreateCall(callee_func, argv, "calltmp");
};

llvm::Function *Prototype::codegen() {
    // Convert args to a vector of doubles
    std::vector<llvm::Type *> doubles(
        this->args.size(), llvm::Type::getDoubleTy(*LLVMComponents::context));
    // Create llvm function
    auto *type = llvm::FunctionType::get(
        llvm::Type::getDoubleTy(*LLVMComponents::context), doubles, false);

    auto *function =
        llvm::Function::Create(type, llvm::Function::ExternalLinkage,
                               this->name, LLVMComponents::module.get());
    // Rename the llvm function args to the prototype args
    unsigned index = 0;
    for (auto &arg : function->args())
        arg.setName(this->args[index++]);

    return function;
};

llvm::Function *Function::codegen() {
    auto *function =
        LLVMComponents::module->getFunction(this->prototype->get_name());

    if (!function)
        function = this->prototype->codegen();

    if (!function->empty())
        throw std::runtime_error("Function already defined");

    // Create basic block and start insertion
    auto *block =
        llvm::BasicBlock::Create(*LLVMComponents::context, "entry", function);
    LLVMComponents::builder->SetInsertPoint(block);

    // Record the arguments in the symbol table
    LLVMComponents::symbol_table.clear();
    for (auto &arg : function->args())
        LLVMComponents::symbol_table[std::string(arg.getName())] = &arg;

    // Build body
    try {
        llvm::Value *return_value = this->body->codegen();
        LLVMComponents::builder->CreateRet(return_value);

        // Validate the generated code, checking for consistency.
        verifyFunction(*function);

    } catch (std::runtime_error &exception) {
        function->eraseFromParent(); // Might not be needed depending on how
                                     // error is handled
        throw exception;
    }

    return function;
}

} // namespace AST
