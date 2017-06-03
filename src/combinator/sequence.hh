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

#include "parser/parsers.hh"

namespace Parsing {
namespace combinator
{
    template <typename Parser1, typename Parser2>
    class sequence_t
    {
        Parser1 const p1;
        Parser2 const p2;

    public:
        using is_parser_type = std::true_type;
        using is_handle_type = std::false_type;
        using has_side_effects = std::integral_constant
            <bool, Parser1::has_side_effects::value
                || Parser2::has_side_effects::value>;
        using result_type = typename least_general
            <Parser1, Parser2>::result_type;
        int const rank = 0;

        constexpr sequence_t(
                Parser1 const& p1, Parser2 const& p2)
            : p1(p1), p2(p2)
        {}

        template <typename Iterator, typename Range,
                  typename Inherit = default_inherited>
        bool operator()(
                Iterator &i,
                Range const &r,
                result_type *result = nullptr,
                Inherit *st = nullptr) const
        {
            return p1(i, r, result, st) && p2(i, r, result, st);
        }

        std::string ebnf(dictionary_t* defs = nullptr) const
        {
            return format_name(p1, rank, defs) + ", "
                 + format_name(p2, rank, defs);
        }
    };
}}
