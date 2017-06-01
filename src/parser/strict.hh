#pragma once

#include "parser/parsers.hh"

namespace Parsing {
namespace parser
{
    template <typename Parser>
    class strict_t
    {
        Parser const p;
        char const* err;

        public:
            using is_parser_type = std::true_type;
            using is_handle_type = std::false_type;
            using has_side_effects = typename Parser::has_side_effects;
            using result_type = typename Parser::result_type;
            int const rank;

            constexpr parser_strict(char const* s, Parser const& q)
                : p(q)
                , err(s)
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
                Iterator const first = i;
                if (!p(i, r, result, st)) {
                    throw error(err, p, first, i, r);
                }
                return true;
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
    constexpr strict_t<P> strict(char const* s, P const& p)
    {
        return strict_t<P>(s, p);
    }
}}
