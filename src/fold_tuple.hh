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
#include <utility>
#include <tuple>

namespace Parsing
{
    template <typename Function, typename Acc, typename Tuple, size_t I0>
    Acc fold_tuple2(Function f, Acc a, Tuple &&t, size_t) {
        return f(a, std::get<I0>(t));
    }

    template <typename Function, typename Acc, typename Tuple, size_t I0, size_t... Is>
    Acc fold_tuple2(Function f, Acc a, Tuple&& t, size_t, size_t...) {
        return fold_tuple2<Function, Acc, Tuple, Is...>(
            f, f(a, std::get<I0>(t)), t, Is...);
    }

    template <typename Function, typename Acc, typename Tuple, size_t... Is>
    Acc fold_tuple1(Function f, Acc a, Tuple &&t, std::index_sequence<Is...>) {
        return fold_tuple2<Function, Acc, Tuple, Is...>(f, a, t, Is...);
    }

    /*! \brief Fold over a tuple
     *
     *  \param f Function of two parameters of type Acc and tuple element,
     *           returning Acc.
     *  \param a Accumulator, start value for fold.
     *  \param t Tuple
     */
    template <typename Function, typename Acc, typename... Ts>
    Acc fold_tuple(Function f, Acc a, std::tuple<Ts...> const& t) {
        return fold_tuple1(f, a, t, std::make_index_sequence<sizeof...(Ts)>());
    }
}
