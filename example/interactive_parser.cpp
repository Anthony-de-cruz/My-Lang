#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

#include "lexer.h"
#include "llvm_components.h"
#include "parser.h"

int main() {

    LLVMComponents::initialise_module();

    auto *lexer = new Lexer;
    auto *stream = &std::cin;
    auto *parser = new Parser(lexer, stream);

    std::cout << "> ";

    bool running = true;
    while (running) {
        try {
            switch (parser->handle_expression()) {
            case ';':
                std::cout << "> ";
                break;
            case Lexer::tok_eof:
                std::cout << "EOF" << '\n';
                running = false;
                break;
            }
        } catch (std::runtime_error &exception) {
            std::cerr << exception.what() << '\n';
            break;
        }
    }

    // Print the generated code
    LLVMComponents::module->print(llvm::errs(), nullptr);

    return EXIT_SUCCESS;
}
