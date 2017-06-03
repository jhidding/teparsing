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

#include <type_traits>
#include <tuple>
#include "parser/parsers.hh"

namespace Parsing
{
    //----------------------------------------------------------------------------
    // If all parsers succeed, pass all results as arguments to user supplied functor

    template <typename Functor, typename Inherit, typename = void>
    class call_all {};

    template <typename Functor, typename Inherit>
    class call_all
        <Functor,
         Inherit,
         typename std::enable_if
             <std::is_same<Inherit, default_inherited>::value>::type>
    {
        // typename function_traits<Functor>::function_type const f;
        Functor const f;

        public:
            explicit call_all(Functor const& f)
                : f(f)
            {}

            template <typename Result, typename Rs, size_t... I>
            void all(Result *r, Rs &rs, Inherit *st, size_t...)
            {
                f(r, std::get<I>(rs)...);
            }
    };

    template <typename Functor, typename Inherit>
    class call_all
        <Functor,
         Inherit,
         typename std::enable_if
             <!std::is_same<Inherit, default_inherited>::value>::type>
    {
        // typename function_traits<Functor>::function_type const f;
        Functor const f;

        public:
            explicit call_all(Functor const& f)
                : f(f)
            {}

            template <typename Result, typename Rs, size_t... I>
            void all(Result *r, Rs &rs, Inherit *st, size_t...)
            {
                f(r, std::get<I>(rs)..., st);
            }
    };
}
