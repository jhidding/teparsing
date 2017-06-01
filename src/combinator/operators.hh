#pragma once

#include "combinator/many.hh"
#include "combinator/sequence.hh"
#include "combinator/choice.hh"
#include "combinator/except.hh"

namespace Parsing
{
    template <typename P1,
              typename P2,
              typename = typename std::enable_if
                  <std::is_same<typename P1::is_parser_type, std::true_type>::value
                || std::is_same<typename P1::is_handle_type, std::true_type>::value>::type,
              typename = typename std::enable_if
                  <std::is_same<typename P2::is_parser_type, std::true_type>::value
                || std::is_same<typename P2::is_handle_type, std::true_type>::value>::type,
              typename = typename std::enable_if
                  <is_compatible_bidi<typename P1::result_type,
                                      typename P2::result_type>::value,
                   std::pair<typename P1::result_type, typename P2::result_type>>::type>
    constexpr combinator::choice_t<P1, P2> operator|(
            P1 const& p1, P2 const& p2)
    {
        return combinator::choice_t<P1, P2>(p1, p2);
    }

    template <typename P1,
              typename P2,
              typename = typename std::enable_if
                  <std::is_same<typename P1::is_parser_type, std::true_type>::value
                || std::is_same<typename P1::is_handle_type, std::true_type>::value>::type,
              typename = typename std::enable_if
                  <std::is_same<typename P2::is_parser_type, std::true_type>::value
                || std::is_same<typename P2::is_handle_type, std::true_type>::value>::type,
              typename = typename std::enable_if
                  <is_compatible_bidi<typename P1::result_type,
                                      typename P2::result_type>::value>::type>
    constexpr combinator::sequence_t<P1, P2> operator&(
            P1 const& p1, P2 const& p2)
    {
        return combinator::sequence_t<P1, P2>(p1, p2);
    }

    template <typename P,
              typename = typename std::enable_if
                  <std::is_same<typename P::is_parser_type, std::true_type>::value
                || std::is_same<typename P::is_handle_type, std::true_type>::value>::type>
    constexpr combinator::many_t<P> many(P const& p)
    {
        return combinator::many_t<P>(p);
    }

    template <typename P,
              typename = typename std::enable_if
                  <std::is_same<typename P::is_parser_type, std::true_type>::value
                || std::is_same<typename P::is_handle_type, std::true_type>::value>::type>
    constexpr combinator::except_t<P> operator-(P const& p, char const* x)
    {
        return combinator::except_t<P>(x, p);
    }
}
