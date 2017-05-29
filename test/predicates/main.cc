#include <gtest/gtest.h>
#include <string>

#include "parsing.hh"

using namespace Parsing;

TEST(Predicates, Any)
{
    using predicate::any;

    EXPECT_TRUE(any<char>('c'));
    EXPECT_TRUE(any<char>('~'));
    EXPECT_TRUE(any<char>('*'));
    EXPECT_FALSE(any<char>(std::char_traits<char>::eof()));

    EXPECT_EQ(any<char>.name(), "anything");
}

TEST(Predicates, Equal)
{
    using predicate::equal;

    EXPECT_TRUE(equal('q')('q'));
    EXPECT_FALSE(equal('a')('b'));
    EXPECT_EQ(equal('z').name(), "'z'");
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
