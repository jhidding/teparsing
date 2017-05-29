#pragma once

/* Based on Parser-Combinators by Keean Schupke */

#include <type_traits>
#include <string>

namespace Parsing
{
    namespace predicate
    {
        template <typename char_t>
        struct any_t
        {
            using is_predicate_type = std::true_type;
            static constexpr int rank = 0;

            constexpr any_t() {}

            bool operator()(char_t c) const
            {
                return c != std::char_traits<char_t>::eof();
            }

            std::string name() const
            {
                return "anything";
            }
        };

        template <typename char_t>
        constexpr any_t<char_t> any;

        template <typename char_t>
        struct equal_t
        {
            char_t const value;

            using is_predicate_type = std::true_type;
            static constexpr int rank = 0;

            constexpr equal_t(char_t c)
                : value(c)
            {}

            bool operator()(char_t c) const
            {
                return c == value;
            }
            
            std::string name() const
            {
                return "'" + std::string(1, value) + "'";
            }
        };

        template <typename char_t>
        constexpr equal_t<char_t> equal(char_t c)
        {
            return equal_t<char_t>(c);
        }
    }
}

