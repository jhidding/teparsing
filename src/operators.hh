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

#include "predicate/predicates.hh"
#include "combinator/many.hh"
#include "combinator/sequence.hh"
#include "combinator/choice.hh"
#include "combinator/except.hh"

namespace Parsing
{
    template <typename P1,
              typename P2,
              typename = typename P1::is_predicate_type,
              typename = typename P2::is_predicate_type>
    constexpr predicate::either_t<P1, P2> operator|(P1 const &p1, P2 const &p2)
    {
        return predicate::either_t<P1, P2>(p1, p2);
    }

    template <typename P1,
              typename P2,
              typename = typename P1::is_predicate_type,
              typename = typename P2::is_predicate_type>
    constexpr predicate::except_t<P1, P2> operator-(P1 const &p1, P2 const &p2)
    {
        return predicate::except_t<P1, P2>(p1, p2);
    }

    template <typename P1,
              typename P2,
              typename = typename std::enable_if
                  <std::is_same<typename P1::is_parser_type, std::true_type>::value
                || std::is_same<typename P1::is_handle_type, std::true_type>::value>::type,
              typename = typename std::enable_if
                  <std::is_same<typename P2::is_parser_type, std::true_type>::value
                || std::is_same<typename P2::is_handle_type, std::true_type>::value>::type,
              typename = typename std::enable_if
                  <is_compatible_bidi<typename P1::result_type,
                                      typename P2::result_type>::value,
                   std::pair<typename P1::result_type, typename P2::result_type>>::type>
    constexpr combinator::choice_t<P1, P2> operator|(
            P1 const& p1, P2 const& p2)
    {
        return combinator::choice_t<P1, P2>(p1, p2);
    }

    template <typename P1,
              typename P2,
              typename = typename std::enable_if
                  <std::is_same<typename P1::is_parser_type, std::true_type>::value
                || std::is_same<typename P1::is_handle_type, std::true_type>::value>::type,
              typename = typename std::enable_if
                  <std::is_same<typename P2::is_parser_type, std::true_type>::value
                || std::is_same<typename P2::is_handle_type, std::true_type>::value>::type,
              typename = typename std::enable_if
                  <is_compatible_bidi<typename P1::result_type,
                                      typename P2::result_type>::value>::type>
    constexpr combinator::sequence_t<P1, P2> operator&(
            P1 const& p1, P2 const& p2)
    {
        return combinator::sequence_t<P1, P2>(p1, p2);
    }

    template <typename P,
              typename = typename std::enable_if
                  <std::is_same<typename P::is_parser_type, std::true_type>::value
                || std::is_same<typename P::is_handle_type, std::true_type>::value>::type>
    constexpr combinator::many_t<P> many(P const& p)
    {
        return combinator::many_t<P>(p);
    }

    template <typename P,
              typename = typename std::enable_if
                  <std::is_same<typename P::is_parser_type, std::true_type>::value
                || std::is_same<typename P::is_handle_type, std::true_type>::value>::type>
    constexpr combinator::except_t<P> operator-(P const& p, char const* x)
    {
        return combinator::except_t<P>(x, p);
    }
}
