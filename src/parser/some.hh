#pragma once

#include "parser/rename.hh"
#include "combinator/operators.hh"

namespace Parsing {
    template <typename P>
    constexpr auto some(P const& p)
    {
        return rename([p] (dictionary_t *defs)
        {
            return "{" + p.ebnf(defs) + "}-";
        }, 0, p & many(p));
    }
}
