#include <gtest/gtest.h>

#include <cstdio>
#include <sstream>
#include <stdexcept>

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
    };
};

TEST_F(ParserTest, BasicExpression) {
    *stream << "x+y;";
    try {
        EXPECT_EQ(parser->handle_expression(), 59);
        EXPECT_EQ(lexer->get_identifier(), "y");
    } catch (std::runtime_error &exception) {
        FAIL() << exception.what() << '\n';
    }
}

TEST_F(ParserTest, BasicException) {
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
