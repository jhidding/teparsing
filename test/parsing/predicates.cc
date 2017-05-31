/* Copyright 2017 Johan Hidding
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
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

TEST(Predicates, Either)
{
    using predicate::equal;

    auto a = equal('p') | equal('q');
    EXPECT_TRUE(a('q'));
    EXPECT_TRUE(a('p'));
    EXPECT_FALSE(a('r'));

    EXPECT_EQ(a.name(), "'p' | 'q'");
}

TEST(Predicates, Except)
{
    using predicate::equal;

    auto a = equal('p') | equal('q') | equal('r');
    auto b = a - equal('q');

    EXPECT_TRUE(a('q'));
    EXPECT_TRUE(a('r'));
    EXPECT_FALSE(b('q'));
    EXPECT_TRUE(b('r'));
    EXPECT_EQ(b.name(), "('p' | 'q' | 'r') - 'q'");
}
