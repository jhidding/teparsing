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

#include "fold_tuple.hh"

using namespace Parsing;

struct A
{
    int x;
    A(int x): x(x) {}
};

struct B
{
    int x;
    B(int x): x(x) {}
};

TEST(Functional, PolymorphicFold)
{
    auto a = std::make_tuple(A(2), B(3), A(4));
    int y = fold_tuple(
        [] (int t, auto q) { return t * q.x; },
        1, a);

    EXPECT_EQ(y, 24);
}
