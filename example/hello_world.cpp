#include "lexer.h"
#include <cstdlib>
#include <iostream>

int main() {
    auto lexer = new Lexer;

    std::cout << lexer->get_token() << '\n';
    return EXIT_SUCCESS;
}
