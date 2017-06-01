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

#include "parser/parsers.hH"

namespace Parsing {
namespace parser
{
    template <typename Parser>
    class log_t
    {
        Parser const p;
        string const msg;

        public:
            using is_parser_type = std::true_type;
            using is_handle_type = std::false_type;
            using has_side_effects = typename Parser::has_side_effects;
            using result_type = typename Parser::result_type;
            int const rank;

            constexpr explicit log_t(string const &s, Parser const& q)
                : p(q)
                , msg(s)
                , rank(q.rank)
            {}

            template <typename Iterator, typename Range,
                      typename Inherit = default_inherited>
            bool operator() (
                    Iterator &i,
                    Range const &r,
                    result_type *result = nullptr,
                    Inherit *st = nullptr) const
            {
                Iterator const x = i;

                bool const b = p(i, r, result, st);

#ifdef DEBUG
                if (b) {
                    std::cerr << msg << ": ";

                    if (result != nullptr) {
                        std::cerr << *result;
                    }

                    std::cerr << " @" << (x - r.first) << " - " << (i - r.first) << "\n";
                }
#endif

                return b;
            }

            std::string ebnf(dictionary_t *defs = nullptr) const
            {
                return p.ebnf(defs);
            }
    };

    template <typename P,
              typename = typename std::enable_if
                  <std::is_same<typename P::is_parser_type, std::true_type>::value
                || std::is_same<typename P::is_handle_type, std::true_type>::value>::type>
    constexpr log_t<P> log(std::string const &s, P const& p)
    {
        return log_t<P>(s, p);
    }
}}
