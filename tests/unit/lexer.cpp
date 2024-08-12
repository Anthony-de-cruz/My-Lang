#include <gtest/gtest.h>

#include <cstdio>
#include <sstream>

#include "lexer.h"

class LexerTest : public testing::Test {
  protected:
    Lexer *lexer;
    std::stringstream *stream;

    LexerTest() {
        lexer = new Lexer;
        stream = new std::stringstream;
    };
};

TEST_F(LexerTest, Eof) { ASSERT_EQ(lexer->get_token(stream), lexer->tok_eof); }

TEST_F(LexerTest, Def) {
    *stream << "def";

    ASSERT_EQ(lexer->get_token(stream), lexer->tok_def);
    ASSERT_EQ(lexer->get_identifier(), "def");
}

TEST_F(LexerTest, Extern) {
    *stream << "extern";

    ASSERT_EQ(lexer->get_token(stream), lexer->tok_extern);
    ASSERT_EQ(lexer->get_identifier(), "extern");
}

TEST_F(LexerTest, Identifier) {
    *stream << "test";

    ASSERT_EQ(lexer->get_token(stream), lexer->tok_identifier);
    ASSERT_EQ(lexer->get_identifier(), "test");
}

TEST_F(LexerTest, Integer) {
    *stream << "123";

    ASSERT_EQ(lexer->get_token(stream), lexer->tok_number);
    ASSERT_EQ(lexer->get_numeric(), 123);
}

TEST_F(LexerTest, Decimal) {
    *stream << "123.4";

    ASSERT_EQ(lexer->get_token(stream), lexer->tok_number);
    ASSERT_EQ(lexer->get_numeric(), 123.4);
}

TEST_F(LexerTest, Unknown) {
    *stream << "+";

    ASSERT_EQ(lexer->get_token(stream), 43);
}

TEST_F(LexerTest, Comment) {
    *stream << "# wee hello hi";

    ASSERT_EQ(lexer->get_token(stream), Lexer::tok_eof);
}

TEST_F(LexerTest, StringOfTokens) {
    *stream << "test string 123.3 #comment";

    ASSERT_EQ(lexer->get_token(stream), Lexer::tok_identifier);
    ASSERT_EQ(lexer->get_identifier(), "test");

    ASSERT_EQ(lexer->get_token(stream), Lexer::tok_identifier);
    ASSERT_EQ(lexer->get_identifier(), "string");

    ASSERT_EQ(lexer->get_token(stream), Lexer::tok_number);
    ASSERT_EQ(lexer->get_numeric(), 123.3);

    ASSERT_EQ(lexer->get_token(stream), Lexer::tok_eof);
}

TEST_F(LexerTest, FunctionDef) {
    *stream << "def my_func(x)";

    ASSERT_EQ(lexer->get_token(stream), Lexer::tok_def);

    ASSERT_EQ(lexer->get_token(stream), Lexer::tok_identifier);
    ASSERT_EQ(lexer->get_identifier(), "my_func");

    ASSERT_EQ(lexer->get_token(stream), 40);

    ASSERT_EQ(lexer->get_token(stream), Lexer::tok_identifier);
    ASSERT_EQ(lexer->get_identifier(), "x");

    ASSERT_EQ(lexer->get_token(stream), 41);
}
