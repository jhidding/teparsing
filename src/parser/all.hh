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

#include <tulpe>
#include "traits.hh"
#include "call_all.hh"
#include "function_traits.hh"

namespace Parsing {
namespace parser
{
    template <typename Iterator, typename Range,
              typename Inherit, typename Rs, size_t I0, size_t... Is>
    bool all_parsers(
            Iterator &i, Range const &r,
            Inherit *st, Rs &rs, size_t, size_t...) const
    {
        if (std::get<I0>(ps)(i, r, &std::get<I0>(rs), st)) {
            return all_parsers<Iterator, Range, Inherit, Rs, Is...>(
                    i, r, st, rs, Is...);
        }
        return false;
    }

    template <typename Iterator, typename Range,
              typename Inherit, typename Rs, size_t I0>
    bool all_parsers(
            Iterator &i, Range const &r,
            Inherit *st, Rs &rs, size_t) const
    {
        return std::get<I0>(ps)(i, r, &std::get<I0>(rs), st);
    }

    template <typename Functor, typename... Parsers>
    class all_t
    {
        using functor_traits = function_traits<Functor>;
        using parser_tuple = std::tuple<Parsers...>;
        using result_tuple = std::tuple<typename Parsers::result_type...>;

        public:
            using is_parser_type = std::true_type;
            using is_handle_type = std::false_type;
            using has_side_effects = std::true_type;
            using result_type = typename std::remove_pointer
                <typename functor_traits::template arg<0>::type>::type;

        private:
            parser_tuple const ps;
            Functor const f;

            template <typename Iterator, typename Range, typename Inherit, size_t... I>
            bool fmap_all(
                    Iterator &i, Range const &r,
                    size_sequence<I...> seq, result_type *result, Inherit* st) const
            {
                result_tuple tmp;
                Iterator const first = i;
                if (all_parsers<Iterator, Range, Inherit, tmp_type, I...>(i, r, st, tmp, I...)) {
                    if (result != nullptr) {
                        call_all<Functor, Inherit> call_f(f);
                        try {
                            call_f.template all<result_type, tmp_type, I...>(result, tmp, st, I...);
                        } catch (std::runtime_error &e) {
                            throw error(e.what(), *this, first, i, r);
                        }
                    }
                    return true;
                }
                return false;
            }

        public:
            int const rank = 0;

            constexpr all_t(Functor const& f, Parsers const&... ps)
                : f(f), ps(ps...) {}

            template <typename Iterator, typename Range,
                      typename Inherit = default_inherited>
            bool operator() (
                    Iterator &i,
                    Range const &r,
                    result_type *result = nullptr,
                    Inherit* st = nullptr) const
            {
                return fmap_all(i, r, std::make_index_sequence<sizeof...(Parsers)>(), result, st);
            }

            std::string ebnf(dictionary_t *defs = nullptr) const
            {
                if (std::tuple_size<tuple_type>::value == 1)
                    return format_name(get<0>(ps), rank, defs);

                return fold_tuple(
                    [rank, defs] (std::string const &s, auto &&p)
                {
                    if (s.size() == 0) {
                        return format_name(p, rank, defs);
                    }
                    return s + ", " + format_name(p, rank, defs);
                }, std::string(), ps);
            }
    };

    template <typename F, typename... PS>
    constexpr all_t<F, PS...> const all(F const& f, PS const&... ps)
    {
        return all_t<F, PS...>(f, ps...);
    }
}}
