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

#include <string>
#include "exceptions.hh"
#include "traits.hh"

namespace Parsing {
namespace parser {
    struct default_inherited {};

    template <typename P,  typename = typename P::is_parser_type>
    std::string format_name(
            P const &p, int const rank, dictionary_t* defs = nullptr)
    {
        if (p.rank > rank) {
            return "(" + p.ebnf(defs) + ")";
        } else {
            return p.ebnf(defs);
        }
    }

    template <typename Predicate>
    class accept_t
    {
        Predicate const p;

        public:
            using char_type = typename Predicate::char_type;
            using is_parser_type = std::true_type;
            using is_handle_type = std::false_type;
            using has_side_effects = std::false_type;
            using result_type = std::string;
            int const rank;

            constexpr explicit accept_t(Predicate const& p)
                : p(p)
                , rank(p.rank)
            {}

            template <typename Iterator,
                      typename Range,
                      typename Inherit = default_inherited>
            bool operator() (
                    Iterator &i,
                    Range const &r,
                    std::string *result = nullptr,
                    Inherit *st = nullptr) const
            {
                char_type sym;

                if (i == r.end()) {
                    sym = std::char_traits<char_type>::eof();
                } else {
                    sym = *i;
                }

                if (!p(sym)) {
                    return false;
                }

                ++i;

                if (result != nullptr) {
                    result->push_back(sym);
                }

                return true;
            }

            std::string ebnf(dictionary_t *defs = nullptr) const
            {
                return p.name();
            }
    };

    /*! \brief Stream is advanced if symbol matches, and symbol is appended
     *         to result.
     */
    template <typename P,
              typename = typename P::is_predicate_type>
    constexpr accept_t<P> accept(P const &p)
    {
        return accept_t<P>(p);
    }

    class accept_string_t {
        char const *s;

        public:
            using is_parser_type = std::true_type;
            using is_handle_type = std::false_type;
            using has_side_effects = std::false_type;
            using result_type = std::string;
            int const rank = 0;

            constexpr explicit accept_string_t(char const *s)
                : s(s)
            {}

            template <typename Iterator,
                      typename Range,
                      typename Inherit = default_inherited>
            bool operator() (
                    Iterator &i,
                    Range const &r,
                    std::string *result = nullptr,
                    Inherit *st = nullptr) const
            {
                for (char const *j = s; *j != 0; ++j)
                {
                    if (i == r.end() || *i != *j) {
                        return false;
                    }
                    ++i;
                }

                if (result != nullptr) {
                    result->append(std::string(s));
                }

                return true;
            }

            std::string ebnf(dictionary_t *defs = nullptr) const
            {
                return "\"" + std::string(s) + "\"";
            }
    };

    inline constexpr accept_string_t accept_string(char const *s)
    {
        return accept_string_t(s);
    }

    class pass_t
    {
        public:
            using is_parser_type = std::true_type;
            using is_handle_type = std::false_type;
            using has_side_effects = std::false_type;
            using result_type = void;
            int const rank = 0;

            constexpr pass_t() {}

            template <typename Iterator,
                      typename Range,
                      typename Inherit = default_inherited>
            bool operator() (
                    Iterator &i,
                    Range const &r,
                    void *result = nullptr,
                    Inherit *st = nullptr) const
            {
                return true;
            }

            std::string ebnf(dictionary_t *defs = nullptr) const
            {
                return "pass";
            }
    };

    /*! \brief Always passes
     */
    constexpr pass_t pass;

    class fail_t
    {
        public:
            using is_parser_type = std::true_type;
            using is_handle_type = std::false_type;
            using has_side_effects = std::false_type;
            using result_type = void;
            int const rank = 0;

            constexpr fail_t() {}

            template <typename Iterator,
                      typename Range,
                      typename Inherit = default_inherited>
            bool operator() (
                    Iterator &i,
                    Range const &r,
                    void *result = nullptr,
                    Inherit *st = nullptr) const
            {
                return false;
            }

            std::string ebnf(dictionary_t *defs = nullptr) const {
                return "fail";
            }
    };

    /*! \brief Always fails
     */
    constexpr fail_t fail;
}}
