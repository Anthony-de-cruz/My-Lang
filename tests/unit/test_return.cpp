#include <gtest/gtest.h>

#include "lexer.h"

class LexerTest : public testing::Test {
  protected:
};

TEST(LexerTest, BasicTest) { ASSERT_EQ(lexer::return_1(), 1); }
