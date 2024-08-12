#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

#include "ir_builder.h"
#include "lexer.h"
#include "parser.h"

int main() {

    IRBuilder::initialise_module();

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
    IRBuilder::module->print(llvm::errs(), nullptr);

    return EXIT_SUCCESS;
}
