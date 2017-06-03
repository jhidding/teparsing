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
#include <type_traits>
#include "parser/parsers.hh"

namespace Parsing
{
    //----------------------------------------------------------------------------
    // as soon as one parser succeeds, pass result to user supplied functor

    template <typename Functor,
              typename Inherit,
              typename = void>
    class call_any {};

    template <typename Functor,
              typename Inherit>
    class call_any
        <Functor,
         Inherit,
         typename std::enable_if
             <std::is_same<Inherit, default_inherited>::value>::type>
    {
        Functor const f;

        public:
            explicit call_any(Functor const& f)
                : f(f)
            {}

            template <typename Result, typename Rs, size_t... I>
            void any(Result *r, int j, Rs &rs, Inherit *st, size_t...) {
                f(r, j, std::get<I>(rs)...);
            }
    };

    template <typename Functor,
              typename Inherit>
    class call_any
        <Functor,
         Inherit,
         typename std::enable_if
             <!std::is_same<Inherit, default_inherited>::value>::type>
    {
        Functor const f;

        public:
            explicit call_any(Functor const& f)
                : f(f)
            {}

            template <typename Result, typename Rs, size_t... I>
            void any(Result* r, int j, Rs &rs, Inherit* st, size_t...) {
                f(r, j, std::get<I>(rs)..., st);
            }
    };
}
