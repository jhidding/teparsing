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
    using parser::default_inherited;

    template <typename Parser>
    class many_t
    {
        Parser const p;

        public:
            using is_parser_type = std::true_type;
            using is_handle_type = std::false_type;
            using has_side_effects = typename Parser::has_side_effects;
            using result_type = typename Parser::result_type;
            int const rank = 0;

            constexpr explicit many_t(Parser const& p)
                : p(p)
            {}

            template <typename Iterator, typename Range,
                      typename Inherit = default_inherited>
            bool operator() (
                    Iterator &i,
                    Range const &r,
                    result_type *result = nullptr,
                    Inherit *st = nullptr) const
            {
                Iterator first = i;
                while (p(i, r, result, st)) {
                    first = i;
                }
                if (first != i) {
                    throw parse_error("failed many-parser consumed input", p, first, i, r);
                }
                return true;
            }

            std::string ebnf(dictionary_t* defs = nullptr) const
            {
                return "{" + p.ebnf(defs) + "}";
            }
    };
}}
