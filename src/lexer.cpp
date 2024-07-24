#include <cctype>
#include <cstdio>

#include "lexer.h"

enum Lexer::Token Lexer::get_token() {

    int last_char = ' ';

    while (isspace(last_char))
        last_char = getchar();

    // Identifier: [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(last_char)) {
        // Consume whole word
        identifier = last_char;
        while (isalnum(last_char = getchar()))
            identifier += last_char;

        if (identifier == "def")
            return tok_def;
        if (identifier == "extern")
            return tok_extern;

        return tok_identifier;
    }

    // Number: [0-9.]+
    if (isdigit(last_char) || last_char == '.') {
        std::string num_string;
        do {
            num_string += last_char;
            last_char = getchar();
        } while (isdigit(last_char) || last_char == '.');

        numeric = strtod(num_string.c_str(), nullptr);
        return tok_number;
    }

    // Comments until \n
    if (last_char == '#') {
        do
            last_char = getchar();
        while (last_char != EOF && last_char != '\n' && last_char != '\r');

        if (last_char != EOF)
            return get_token();
    }

    if (last_char == EOF)
        return tok_eof;

    // Return unknown chars
    int current_char = last_char;
    last_char = getchar();
    return Lexer::Token(current_char);
}
