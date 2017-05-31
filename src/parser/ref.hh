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
    template <typename Parser>
    class ref_t
    {
        Parser const *p;
        char const* name;

        public:
            using is_parser_type = std::true_type;
            using is_handle_type = std::false_type;
            using has_side_effects = typename Parser::has_side_effects;
            using result_type = typename Parser::result_type;
            int const rank = 0;

            constexpr ref_t(char const* name, Parser const* q)
                : p(q)
                , name(name)
            {}

            template <typename Iterator, typename Range,
                      typename Inherit = default_inherited>
            bool operator()(
                    Iterator &i, Range const &r,
                    result_type *result = nullptr,
                    Inherit *st = nullptr) const
            {
                return (*p)(i, r, result, st);
            }

            std::string ebnf(dictionary *defs = nullptr) const
            {
                if (defs != nullptr) {
                    auto i = defs->find(name);
                    if (i == defs->end()) {
                        auto i = (defs->emplace(name, name)).first;
                        string const n = p->ebnf(defs);
                        i->second = n;
                    }
                }
                return name;
            }
    };

    template <typename P,
              typename = typename P::is_parser_type>
    constexpr ref_t<P> ref(char const *name, P const* p)
    {
        return ref_t<P>(name, p);
    }
}}
