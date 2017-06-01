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
namespace parser
{
    template <typename F>
    class fix_t
    {
        using parser_type = typename function_traits<F>::return_type;
        parser_type const p;
        char const* name;

        public:
            using is_parser_type = std::true_type;
            using is_handle_type = std::false_type;
            using has_side_effects = typename parser_type::has_side_effects;
            using result_type = typename parser_type::result_type;
            int const rank = 0;

            constexpr fix_t(char const *n, F f)
                : p(f(ref(n, &p)))
                , name(n)
            {}

            template <typename Iterator, typename Range,
                      typename Inherit = default_inherited>
            bool operator()(
                    Iterator &i,
                    Range const &r,
                    result_type *result = nullptr,
                    Inherit *st = nullptr) const
            {
                return p(i, r, result, st);
            }

            std::string ebnf(dictionary_t *defs = nullptr) const
            {
                std::string const n = p.ebnf(defs);
                if (defs != nullptr) {
                    defs->emplace(name, n);
                }
                return name;
            }
    };

    template <typename F> // , typename = typename function_traits<F>::result_type::is_parser_type>
    constexpr fix_t<F> fix(char const* n, F f)
    {
        return fix_t<F>(n, f);
    }
}}
