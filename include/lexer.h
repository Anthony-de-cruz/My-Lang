#ifndef LEXER_H
#define LEXER_H

#include <istream>
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
    enum Token get_token(std::istream *input_stream);

    /**
     * The string of the last identifier token captured.
     */
    inline std::string &get_identifier() { return identifier; }

    /**
     * The value of the last numeric token captured.
     */
    inline double get_numeric() const { return numeric; };
};

#endif // !LEXER_H
