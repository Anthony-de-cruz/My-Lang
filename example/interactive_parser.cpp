#include <cstdlib>
#include <iostream>

#include "lexer.h"
#include "parser.h"

int main() {

    Lexer *lexer = new Lexer;
    Parser *parser = new Parser(lexer, &std::cin);

    // Prime the first token.
    fprintf(stderr, "ready> ");
    parser->get_next_token();

    // Run the main "interpreter loop" now.
    parser->main_loop();

    return EXIT_SUCCESS;
}
