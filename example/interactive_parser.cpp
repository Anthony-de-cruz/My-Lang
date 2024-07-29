#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "lexer.h"
#include "parser.h"

int main() {

    Lexer *lexer = new Lexer;
    Parser *parser = new Parser(lexer, &std::cin);

    // Prime the first token.
    fprintf(stderr, "ready> ");
    parser->get_next_token();

    while (true) {
        try {
            fprintf(stderr, "ready> ");
            parser->handle_expression();
        } catch (std::runtime_error &exception) {
            std::cerr << exception.what() << '\n';
            parser->get_next_token();
        }
    }

    return EXIT_SUCCESS;
}
