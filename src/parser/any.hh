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

#include <tuple>
#include "traits.hh"
#include "call_any.hh"
#include "function_traits.hh"
#include "exceptions.hh"

namespace Parsing {
namespace parser
{
    template <typename Iterator, typename Range, typename Inherit, typename Rs, size_t I0, size_t... Is>
    int any_parsers(Iterator &i, Range const &r, Inherit *st, Rs &rs, size_t, size_t...) const {
        if (std::get<I0>(ps)(i, r, &std::get<I0>(rs), st)) {
            return I0;
        }
        return any_parsers<Iterator, Range, Inherit, Rs, Is...>(i, r, st, rs, Is...);
    }

    template <typename Iterator, typename Range, typename Inherit, typename Rs, size_t I0>
    int any_parsers(Iterator &i, Range const &r, Inherit *st, Rs &rs, size_t) const {
        if (std::get<I0>(ps)(i, r, &std::get<I0>(rs), st)) {
            return I0;
        }
        return -1;
    }

    template <typename Functor, typename... Parsers>
    class any_t
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

        template <typename Iterator,
                  typename Range,
                  typename Inherit,
                  size_t... I>
        bool fmap_any(
                Iterator &i,
                Range const &r,
                size_sequence<I...> seq,
                result_type *result,
                Inherit* st) const
        {
            result_tuple tmp;
            Iterator const first = i;

            int const j = any_parsers<Iterator, Range, Inherit, tmp_type, I...>(
                    i, r, st, tmp, I...);

            if (j >= 0) {
                if (result != nullptr) {
                    call_any<Functor, Inherit> call_f(f);
                    try {
                        call_f.template any<result_type, tmp_type, I...>(
                            result, j, tmp, st, I...);
                    } catch (runtime_error &e) {
                        throw error(e.what(), *this, first, i, r);
                    }
                }
                return true;
            }
            return false;
        }

    public:
        int const rank = 1;

        constexpr explicit any_t(
                Functor const &f, Parsers const &...ps)
            : f(f)
            , ps(ps...)
        {}

        template <typename Iterator, typename Range, typename Inherit>
        bool operator() (
                Iterator &i,
                Range const &r,
                result_type *result = nullptr,
                Inherit *st = nullptr) const
        {
            return fmap_any(
                i, r, std::make_index_sequence<sizeof...(Parsers)>(),
                result, st);
        }

        string ebnf(dictionary *defs = nullptr) const
        {
            return fold_tuple(
                [rank, defs] (std::string const &s, auto &&p)
            {
                if (s.length() == 0) {
                    return format_name(p, rank, defs);
                }
                return s + " | " + format_name(p, rank, defs);
            }, std::string(), ps);
        }
    };

    template <typename F, typename... PS>
    constexpr any_t<F, PS...> const any(F const& f, PS const&... ps) {
        return any_t<F, PS...>(f, ps...);
    }
}}
