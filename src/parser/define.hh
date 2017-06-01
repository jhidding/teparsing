#pragma once

#include "parser/parsers.hh"

namespace Parsing {
namespace parser
{
    template <typename Parser>
    class define_t
    {
        Parser const p;
        char const *name;

        public:
            using is_parser_type = std::true_type;
            using is_handle_type = std::false_type;
            using has_side_effects = typename Parser::has_side_effects;
            using result_type = typename Parser::result_type;
            int const rank;

            constexpr define_t(char const* n, Parser const& q)
                : p(q)
                , rank(q.rank)
                , name(n)
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
                std::string const n = p.ebnf(defs);
                if (defs != nullptr) {
                    defs->emplace(name, n);
                }
                return name;
            }
    };

    template <typename P,
              typename = typename std::enable_if
                  <std::is_same<typename P::is_parser_type, std::true_type>::value
                || std::is_same<typename P::is_handle_type, std::true_type>::value>::type>
    constexpr define_t<P> define(char const* s, P const &p)
    {
        return define_t<P>(s, p);
    }
}}
