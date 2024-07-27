#ifndef PARSER_H
#define PARSER_H

#include <memory>

#include "ast.h"
#include "lexer.h"

class Parser {

    Lexer *lexer;
    int current_token;

  public:
    Parser(Lexer *lexer) : lexer(lexer) {};

    int get_next_token();

    std::unique_ptr<AST::Expression> parse_expression();

    /**
     * Parse an identifier expression
     */
    std::unique_ptr<AST::Expression> parse_identifier();

    /**
     * Parse a numeric expression
     */
    std::unique_ptr<AST::Number> parse_number();

    /**
     * Parse parentheses
     */
    std::unique_ptr<AST::Expression> parse_parentheses();

    /**
     * Parse any primary expression. These include:
     * - Identifiers
     * - Numbers
     * - Parentheses
     *
     * This acts as a wrapper function around:
     * - parse_identifier()
     * - parse_number()
     * - parse_parentheses()
     */
    std::unique_ptr<AST::Expression> parse_primary();
};

#endif // !PARSER_H
