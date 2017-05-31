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
#pragma once

#include <type_traits>

namespace Parsing
{
    template <typename A, typename B>
    struct is_compatible
    {
        using PA = typename std::add_pointer<A>::type;
        using PB = typename std::add_pointer<B>::type;
        static constexpr bool value = std::is_convertible<PA, PB>::value;
    };

    template <typename A, typename B>
    struct is_compatible_bidi
    {
        static constexpr bool value = is_compatible<A, B>::value
                                   || is_compatible<B, A>::value;
    };

    //-------------------------------------------------------------------------
    // Choose the result type of two parsers which a pointer the other result
    // type can be conveted into a pointer to.  For example given a parsers
    // with result types void and int, the least general is int, because
    // (void*) can be implicitely converted to (int*). If there is no possible
    // implicit converstion result_type is undefined.
    template <typename P1, typename P2, typename = void>
    struct least_general {};

    template <typename P1, typename P2>
    struct least_general
        <P1, P2,
         typename std::enable_if<
                is_compatible<typename P2::result_type,
                              typename P1::result_type>::value
             && !std::is_same<typename P1::result_type,
                              typename P2::result_type>::value>::type>
    {
        using result_type = typename P2::result_type;
    };

    template <typename P1, typename P2>
    struct least_general
        <P1, P2,
         typename std::enable_if<
                is_compatible<typename P1::result_type,
                              typename P2::result_type>::value
             || std::is_same<typename P1::result_type,
                             typename P2::result_type>::value>::type>
    {
        using result_type = typename P1::result_type;
    };
}
