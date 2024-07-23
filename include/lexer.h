#ifndef LEXER_H
#define LEXER_H

#include <string>

class Lexer {
  private:
    std::string identifier;
    double numeric;

  public:
    enum Token {
        tok_eof = -1,
        // commands
        tok_def = -2,
        tok_extern = -3,
        // primary
        tok_identifier = -4,
        tok_number = -5,
    };

    /**
     * Returns a token code [0-255] if captured standard input is unknown,
     * otherwise returns one of the known codes from Token.
     */
    enum Token get_token();

    /**
     * The string of the last identifier token captured.
     */
    inline std::string &get_identifier() { return identifier; }

    /**
     * The value of the last numeric token captured.
     */
    double get_numeric();
};

#endif // !LEXER_H
