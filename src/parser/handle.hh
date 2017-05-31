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

#include "parser/parsers.hh"

namespace Parsing {
namespace parser
{
    /*! \brief Holds a runtime-polymorphic parser. It is mutable so that it can
     *         be defined before the parser is assigned.
     */
    template <typename Iterator, typename Range,
              typename Synthesize = void, typename Inherit = default_inherited>
    class handle_t
    {
        class holder_base_t
        {
            public:
                virtual ~holder_base() {}

                virtual bool parse(
                        Iterator& i,
                        Range const &r,
                        Synthesize *result = nullptr,
                        Inherit *st = nullptr) const = 0;

                virtual std::string ebnf(dictionary *defs = nullptr) const = 0;
        };

        template <typename Parser>
        class holder_t : public holder_base_t
        {
            Parser const p;

            public:
                explicit holder_t(Parser const &q)
                    : p(q) {}

                explicit holder_t(Parser &&q)
                    : p(std::forward<Parser>(q)) {}

                virtual bool parse(
                        Iterator &i,
                        Range const &r,
                        Synthesize *result = nullptr,
                        Inherit *st = nullptr) const override
                {
                    return p(i, r, result, st);
                }

                virtual std::string ebnf(dictionary *defs = nullptr) const override
                {
                    return p.ebnf(defs);
                }
        };

        using holder_ptr = std::shared_ptr<holder_base_t const>;

        holder_ptr p;

    public:
        using is_parser_type = std::false_type;
        using is_handle_type = std::true_type;
        using has_side_effects = std::true_type; // have to assume it does.
        using result_type = Synthesize;
        int const rank = 0;

        constexpr handle_t() {}

        template <typename P,
                  typename = typename std::enable_if
                      <std::is_same<typename P::is_parser_type, std::true_type>::value>::type>
        constexpr handle_t(P const &q)
            : p(new holder_t<P>(q))
        {}

        // note: initialise name before moving q.
        template <typename P,
                  typename = typename std::enable_if
                      <std::is_same<typename P::is_parser_type, std::true_type>::value>::type>
        constexpr handle_t(P &&q)
            : p(new holder_t<P>(std::forward<P>(q)))
        {}

        constexpr handle_t(handle_t &&q)
            : p(std::move(q.p))
        {}

        constexpr handle_t(handle_t const &q)
            : p(q.p)
        {}

        template <typename P,
                  typename = typename std::enable_if
                      <std::is_same<typename P::is_parser_type, std::true_type>::value>::type>
        handle_t &operator=(P const &q)
        {
            p.reset(new holder_t<P>(q));
            return *this;
        }

        handle_t &operator=(handle_t const &q)
        {
            p = q.p;
            return *this;
        }

        template <typename P,
                  typename = typename std::enable_if
                      <std::is_same<typename P::is_parser_type, std::true_type>::value>::type>
        handle_t &operator=(P &&q)
        {
            p.reset(new holder_t<P>(std::forward<P>(q)));
            return *this;
        }

        handle_t &operator=(handle_t &&q)
        {
            p = std::move(q.p);
            return *this;
        }

        bool operator()(
                Iterator &i,
                Range const &r,
                Synthesize *result = nullptr,
                Inherit *st = nullptr) const
        {
            assert(p != nullptr);
            return p->parse(i, r, result, st);
        }

        string ebnf(dictionary *defs = nullptr) const
        {
            return p->ebnf(defs);
        }
    };
}}
