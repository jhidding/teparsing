#pragma once

#include "parser/parsers.hh"

namespace Parsing {
namespace parser
{
    template <typename Parser>
    class attempt_t
    {
        Parser const p;

        public:
            using is_parser_type = std::true_type;
            using is_handle_type = std::false_type;
            using has_side_effects = typename Parser::has_side_effects;
            using result_type = typename Parser::result_type;
            int const rank;

            constexpr explicit attempt_t(Parser const& q)
                : p(q)
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
                if (p(i, r, result, st)) {
                    return true;
                }
                i = first;
                return false;
            }

            std::string ebnf(dictionary *defs = nullptr) const {
                return p.ebnf(defs);
            }
    };

    template <typename Parser>
    class attempt_side_t
    {
        Parser const p;

        public:
            using is_parser_type = std::true_type;
            using is_handle_type = std::false_type;
            using has_side_effects = typename Parser::has_side_effects;
            using result_type = typename Parser::result_type;
            int const rank;

            constexpr explicit attempt_side_t(Parser const& q)
                : p(q)
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
                Inherit inh;
                if (st != nullptr) {
                    inh = *st;
                }
                if (p(i, r, result, st)) {
                    return true;
                }
                i = first;
                if (st != nullptr) {
                    *st = inh;
                }
                return false;
            }

            std::string ebnf(dictionary* defs = nullptr) const {
                return p.ebnf(defs);
            }
    };

    template <typename P,
              typename = typename std::enable_if
                  <(std::is_same<typename P::is_parser_type, std::true_type>::value
                 || std::is_same<typename P::is_handle_type, std::true_type>::value)
                 && std::is_same<typename P::has_side_effects, std::false_type>::value>::type>
    constexpr attempt_t<P> attempt(P const& p)
    {
        return attempt_t<P>(p);
    }

    template <typename P,
              typename = typename std::enable_if
                  <(std::is_same<typename P::is_parser_type, std::true_type>::value
                 || std::is_same<typename P::is_handle_type, std::true_type>::value)
                 && std::is_same<typename P::has_side_effects, std::true_type>::value>::type>
    constexpr attempt_side_t<P> attempt(P const& p)
    {
        return attempt_side_t<P>(p);
    }
}}
