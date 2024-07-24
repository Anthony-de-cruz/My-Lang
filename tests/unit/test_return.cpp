#include <gtest/gtest.h>
#include <sstream>

#include "lexer.h"

class LexerTest : public testing::Test {
  protected:
    Lexer *lexer;

    LexerTest() { lexer = new Lexer; };
};

TEST_F(LexerTest, BasicTest) {

    auto stream = new std::stringstream;

    *stream << "test string";

    ASSERT_EQ(lexer->get_token(stream), -4);
    ASSERT_EQ(lexer->get_identifier(), "test");
    ASSERT_EQ(lexer->get_token(stream), -4);
    ASSERT_EQ(lexer->get_identifier(), "string");
}
