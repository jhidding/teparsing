#pragma once

#include "parser/parsers.hh"

namespace Parsing {
namespace parser
{
    template <typename Parser, typename Name>
    class rename_t
    {
        Parser const p;
        Name const n;

        public:
            using is_parser_type = std::true_type;
            using is_handle_type = std::false_type;
            using has_side_effects = typename Parser::has_side_effects;
            using result_type = typename Parser::result_type;
            int const rank;

            constexpr rename_t(Name &&m, int r, Parser const &q)
                : p(q)
                , n(std::forward<Name>(m))
                , rank(r)
            {}

            template <typename Iterator, typename Range, typename Inherit>
            bool operator() (
                    Iterator &i,
                    Range const &r,
                    result_type *result = nullptr,
                    Inherit *st = nullptr) const
            {
                return p(i, r, result, st);
            }

            std::string ebnf(dictionary_t *defs = nullptr) const
            {
                return n(defs);
            }
    };

    template <typename P, typename N,
              typename = typename std::enable_if
                  <std::is_same<typename P::is_parser_type, std::true_type>::value
                || std::is_same<typename P::is_handle_type, std::true_type>::value>::type>
    constexpr rename_t<P, N> rename(N&& n, int r, P const& p)
    {
        return rename_t<P, N>(std::forward<N>(n), r, p);
    }
}}
