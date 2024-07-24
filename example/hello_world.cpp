#include <cstdlib>
#include <iostream>

#include "lexer.h"

int main() {
    auto lexer = new Lexer;

    bool running = true;

    while (running) {
        std::cout << " > ";
        auto token = lexer->get_token();

        switch (token) {
        case Lexer::tok_eof:
            running = false;
            break;

        case Lexer::tok_def:
            break;

        case Lexer::tok_extern:
            break;

        case Lexer::tok_identifier:
            std::cout << lexer->get_identifier() << '\n';
            break;

        case Lexer::tok_number:
            std::cout << lexer->get_numeric() << '\n';
            break;

        default:
            std::cout << token << '\n';
        }
    }

    delete lexer;

    return EXIT_SUCCESS;
}
