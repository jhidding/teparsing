#include <gtest/gtest.h>
#include <sstream>

#include "predicate/predicates.hh"
#include "parser/parsers.hh"
#include "parser/any.hh"
#include "parser/some.hh"
#include "parser/tokenise.hh"
#include "combinator/operators.hh"
#include "parser/all.hh"

using namespace Parsing;

TEST(Parsers, Accept)
{
    using parser::accept;

    std::string input = "Hello, World!";
    auto p = many(accept(predicate::any<char>));
    std::string output;

    auto i = input.cbegin();
    bool success = p(i, input, &output);

    EXPECT_TRUE(success);
    EXPECT_EQ(input, output);
}

template <typename T>
bool convert(T *value, std::string const &s)
{
    std::istringstream iss(s);
    iss >> *value;
    return !iss.fail();
}

template <typename T>
bool push_back(std::vector<T> *result, T i)
{
    result->push_back(i);
    return true;
}

TEST(Parsers, BuildList)
{
    using parser::accept;
    using parser::all;

    std::string input = "1 23 31 42 75";
    std::vector<int> output;

    auto number = all(convert<int>, tokenise(some(accept(predicate::digit<char>))));
    auto number_list = many(all(push_back<int>, number));

    auto i = input.cbegin();
    bool success = number_list(i, input, &output);

    std::vector<int> expected = { 1, 23, 31, 42, 75 };
    EXPECT_TRUE(success);
    EXPECT_EQ(output, expected);
}
