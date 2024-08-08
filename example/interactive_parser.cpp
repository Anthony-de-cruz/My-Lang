#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

#include "lexer.h"
#include "parser.h"

int main() {

    Lexer *lexer = new Lexer;
    std::istream *stream = &std::cin;

    std::cout << "> ";
    Parser *parser = new Parser(lexer, stream);

    while (true) {
        try {
            switch (parser->handle_expression()) {
            case ';':
                std::cout << "> ";
                break;
            }
        } catch (std::runtime_error &exception) {
            std::cerr << exception.what() << '\n';
            break;
        }
    }

    return EXIT_SUCCESS;
}
