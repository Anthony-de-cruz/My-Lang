#include "llvm_components.h"

namespace LLVMComponents {

std::unique_ptr<llvm::LLVMContext> context;
std::unique_ptr<llvm::IRBuilder<>> builder;
std::unique_ptr<llvm::Module> module;
std::map<std::string, llvm::Value *> symbol_table;

void initialise_module() {
    context = std::make_unique<llvm::LLVMContext>();
    assert(context);
    module = std::make_unique<llvm::Module>("My Lang JIT Compiler", *context);
    assert(module);
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
    assert(builder);
}

} // namespace LLVMComponents
