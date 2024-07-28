#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <memory>

#include "ast.h"
#include "lexer.h"

class Parser {

    Lexer *lexer;
    int current_token;

    static std::map<char, int> binop_precidence;

  public:
    Parser(Lexer *lexer) : lexer(lexer) {};

    int get_next_token();

    /**
     * Parse an identifier expression
     *
     * Throws an exception when parsing a malformed function argument list
     * Propagates any exceptions from parse_expression()
     */
    std::unique_ptr<AST::Expression> parse_identifier();

    /**
     * Parse a numeric expression
     */
    std::unique_ptr<AST::Number> parse_number();

    /**
     * Parse parentheses
     *
     * Throws an exception when an unclosed parentheses is found
     * Propagates any exceptions from parse_expression()
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
     *
     *  Throws an exception when passed an unrecognised expression
     *  Propagates any exceptions from any of the above functions
     */
    std::unique_ptr<AST::Expression> parse_primary();

    /**
     * Parse a primary expression binop
     *
     * Propagates any exceptions from parse_primary()
     */
    std::unique_ptr<AST::Expression> parse_expression();

    /**
     * Parse a primary expression binop pair
     */
    std::unique_ptr<AST::Expression>
    parse_binop_right(int expr_precidence,
                      std::unique_ptr<AST::Expression> left);

    /**
     * Get the precidence of the given binop
     */
    int get_binop_precidence(char binop);
};

#endif // !PARSER_H
