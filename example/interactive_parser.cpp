#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>

#include "lexer.h"
#include "parser.h"

int main() {

    Lexer *lexer = new Lexer;

    // auto stream = new std::stringstream;
    // *stream << "x+y;";
    std::istream *stream = &std::cin;

    std::cout << "> ";
    Parser *parser = new Parser(lexer, stream);

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

    return EXIT_SUCCESS;
}
