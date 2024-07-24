#include <cctype>
#include <cstdio>
#include <iostream>
#include <istream>

#include "lexer.h"

enum Lexer::Token Lexer::get_token(std::istream *input_stream) {

    static int last_char = ' ';

    while (isspace(last_char))
        last_char = input_stream->get();

    // Identifier: [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(last_char)) {
        // Consume whole word
        identifier = last_char;
        while (isalnum(last_char = input_stream->get()) || last_char == '_')
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
            last_char = input_stream->get();
        } while (isdigit(last_char) || last_char == '.');

        numeric = strtod(num_string.c_str(), nullptr);
        return tok_number;
    }

    // Comments until \n
    if (last_char == '#') {
        do
            last_char = input_stream->get();
        while (last_char != EOF && last_char != '\n' && last_char != '\r');

        if (last_char != EOF)
            return get_token(input_stream);
    }

    if (last_char == EOF)
        return tok_eof;

    // Return unknown chars
    int current_char = last_char;
    last_char = input_stream->get();
    return Lexer::Token(current_char);
}
