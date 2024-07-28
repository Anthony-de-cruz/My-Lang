#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "ast.h"
#include "parser.h"

static std::map<char, int> BinopPrecidence{
    {'<', 10}, {'+', 20}, {'-', 20}, {'*', 40}};

int Parser::get_next_token() {
    return current_token = lexer->get_token(&std::cin);
}

std::unique_ptr<AST::Number> Parser::parse_number() {
    auto result = std::make_unique<AST::Number>(lexer->get_numeric());
    get_next_token();
    return result;
}

std::unique_ptr<AST::Expression> Parser::parse_parentheses() {
    get_next_token(); // Consume (
    auto var = parse_expression();
    if (!var)
        return nullptr;

    if (current_token != ')')
        throw std::runtime_error("Syntax Error: Unclosed parentheses");

    get_next_token(); // Consume )
    return var;
}

std::unique_ptr<AST::Expression> Parser::parse_identifier() {
    auto name = lexer->get_identifier();

    get_next_token();

    // If not a function call, treat as a simple variable
    if (current_token != '(')
        return std::make_unique<AST::Variable>(name);

    // Handle as call
    get_next_token(); // Consume (
    std::vector<std::unique_ptr<AST::Expression>> args;

    if (current_token != ')') {
        while (true) {
            auto arg = parse_expression();
            args.push_back(std::move(arg));

            if (current_token == ')')
                break;

            if (current_token != ',')
                throw std::runtime_error(
                    "Syntax Error: expected , or ) in argument list");
            get_next_token();
        }
    }

    get_next_token(); // Consume )

    return std::make_unique<AST::Call>(name, std::move(args));
}

std::unique_ptr<AST::Expression> Parser::parse_primary() {
    switch (current_token) {
    case Lexer::tok_identifier:
        return parse_identifier();
    case Lexer::tok_number:
        return parse_number();
    case '(':
        return parse_parentheses();
    default:
        throw std::runtime_error("Syntax Error: Unknown token " +
                                 std::to_string(current_token));
    }
}

std::unique_ptr<AST::Expression> Parser::parse_expression() {
    auto left = parse_primary();
    return parse_binop_right(0, std::move(left));
}

std::unique_ptr<AST::Expression>
Parser::parse_binop_right(int expr_precidence,
                          std::unique_ptr<AST::Expression> left) {

    // Loop over each expression until
    // they are all merged as a binary expression
    while (true) {
        int token_precidence =
            Parser::get_binop_precidence((char)current_token);

        // If this is a binop that binds at least as tightly as
        // the current binop, consume it, otherwise we are done
        if (token_precidence < expr_precidence)
            return left;

        // Current token must be a binop
        int binop = current_token;
        get_next_token(); // Consume the binop

        // Parse the next expression
        auto right = parse_primary();
        int next_token_precidence =
            Parser::get_binop_precidence((char)current_token);
        if (token_precidence < next_token_precidence) {
            right = parse_binop_right(token_precidence, std::move(right));
        }

        // Merge left and right
        left = std::make_unique<AST::Binary>(binop, std::move(left),
                                             std::move(right));
    }
}

int Parser::get_binop_precidence(char binop) {
    if (binop_precidence.count(binop) == 0)
        throw std::runtime_error("Syntax Error: Invalid binary operator: " +
                                 std::to_string(binop));
    return binop_precidence[binop];
}
