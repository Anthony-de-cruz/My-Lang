#include <cstdio>
#include <gtest/gtest.h>
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

TEST_F(LexerTest, StringOfTokens) {

    *stream << "test string 123.3 #comment" << EOF;

    ASSERT_EQ(lexer->get_token(stream), Lexer::tok_identifier);
    ASSERT_EQ(lexer->get_identifier(), "test");

    ASSERT_EQ(lexer->get_token(stream), Lexer::tok_identifier);
    ASSERT_EQ(lexer->get_identifier(), "string");

    ASSERT_EQ(lexer->get_token(stream), Lexer::tok_number);
    ASSERT_EQ(lexer->get_numeric(), 123.3);

    ASSERT_EQ(lexer->get_token(stream), Lexer::tok_eof);
}
