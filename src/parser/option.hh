#pragma once

#include "parser/rename.hh"
#include "combinator/operators.hh"

namespace Parsing {
namespace parser
{
    template <typename P>
    constexpr auto option(P const& p)
    {
        return rename([p] (dictionary_t *defs)
        {
            return "[" + p.ebnf(defs) + "]";
        }, 0, p | succ);
    }
}}
