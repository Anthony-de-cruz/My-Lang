#include <gtest/gtest.h>

#include <cstdio>
#include <sstream>
#include <stdexcept>

#include "llvm_components.h"
#include "parser.h"

class ParserTest : public testing::Test {
  protected:
    Lexer *lexer;
    std::stringstream *stream;
    Parser *parser;

    ParserTest() {
        lexer = new Lexer;
        stream = new std::stringstream;
        parser = new Parser(lexer, stream);
        LLVMComponents::initialise_module();
    };
};

TEST_F(ParserTest, BasicExpression) {
    *stream << "def increment(x) x+1; increment(5);";
    try {
        EXPECT_EQ(parser->handle_expression(), 59);
        EXPECT_EQ(lexer->get_identifier(), "x");
        EXPECT_EQ(lexer->get_numeric(), 1);
        EXPECT_EQ(parser->handle_expression(), 59);
        EXPECT_EQ(lexer->get_identifier(), "increment");
        EXPECT_EQ(lexer->get_numeric(), 5);
    } catch (std::runtime_error &exception) {
        FAIL() << exception.what() << '\n';
    }
}

TEST_F(ParserTest, MissingOperand) {
    *stream << "x+;";
    ASSERT_THROW(
        {
            try {
                parser->handle_expression();
            } catch (std::runtime_error &exception) {
                EXPECT_STREQ("Syntax Error: Expected primary token, got ;",
                             exception.what());
                throw;
            }
        },
        std::runtime_error);
}

TEST_F(ParserTest, UndeclaredSymbol) {
    *stream << "x+y;";
    ASSERT_THROW(
        {
            try {
                parser->handle_expression();
            } catch (std::runtime_error &exception) {
                EXPECT_STREQ("Use of undeclared symbol: x", exception.what());
                throw;
            }
        },
        std::runtime_error);
}
