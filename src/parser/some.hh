#pragma once

#include "parser/rename.hh"
#include "combinator/operators.hh"

namespace Parsing {
namespace parser {
    template <typename P>
    constexpr auto some(P const& p)
    {
        return rename([p] (dictionary *defs)
        {
            return "{" + p.ebnf(defs) + "}-";
        }, 0, p & many(p));
    }
}}
