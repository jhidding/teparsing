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
#include <type_traits>

namespace Parsing {
namespace predicate
{
    template <typename P,
              typename = typename P::is_predicate_type>
    std::string format_name(P const &p, int rank)
    {
        if (p.rank > rank) {
            return "(" + p.name() + ")";
        } else {
            return p.name();
        }
    }

    template <typename char_t>
    class any_t
    {
    public:
        using char_type = char_t;
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
    class equal_t
    {
        char_t const value;

    public:
        using char_type = char_t;
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

    template <typename P1, typename P2>
    class either_t
    {
        P1 const p1;
        P2 const p2;

        public:
            using char_type = typename P1::char_type;
            using is_predicate_type = std::true_type;
            static constexpr int rank = 1;

            constexpr either_t(P1 const &p1, P2 const &p2)
                : p1(p1)
                , p2(p2)
            {}

            bool operator()(char_type c) const
            {
                return p1(c) || p2(c);
            }

            std::string name() const
            {
                return p1.name() + " | " + p2.name();
            }
    };

    template <typename P1, typename P2>
    class except_t
    {
        P1 const p1;
        P2 const p2;

        public:
            using char_type = typename P1::char_type;
            using is_predicate_type = std::true_type;
            static constexpr int rank = 0;

            constexpr except_t(P1 const &p1, P2 const &p2)
                : p1(p1)
                , p2(p2)
            {}

            bool operator()(char_type c) const
            {
                return p1(c) && !p2(c);
            }

            std::string name() const
            {
                return format_name(p1, rank) + " - " + format_name(p2, rank);
            }
    };
}}
