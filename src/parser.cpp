#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "ast.h"
#include "parser.h"

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
        throw std::runtime_error("Syntax Error: Unclosed parentheses.");

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
            if (auto arg = parse_expression())
                args.push_back(std::move(arg));
            else
                return nullptr;

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
