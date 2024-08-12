#ifndef IR_BUILDER_H
#define IR_BUILDER_H

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <map>
#include <memory>

namespace IRBuilder {

/**
 * llvm::LLVMContext provides the core LLVM data structures, including the type
 * and constant value tables.
 */
extern std::unique_ptr<llvm::LLVMContext> context;

/**
 * llvm::IRBuilder provides a template to build instructions.
 */
extern std::unique_ptr<llvm::IRBuilder<>> builder;

/**
 * llvm::Module is a construct that contains the program's functions and global
 * variables, acting as the owner of all the generated IR.
 */
extern std::unique_ptr<llvm::Module> module;

/**
 * A map to keep track of used symbol names. Function parameters are currently
 * the only symbol names that are kept track of.
 */
extern std::map<std::string, llvm::Value *> symbol_table;

void initialise_module();

} // namespace IRBuilder

#endif // !IR_BUILDER_H
