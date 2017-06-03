#pragma once

#include "parser/rename.hh"
#include "combinator/operators.hh"

namespace Parsing {
namespace parser
{
    template <typename P, typename Q>
    constexpr auto separate_by(P const &p, Q const& q)
    {
        return rename([p, q] (dictionary_t *defs)
        {
            return p.ebnf(defs) + ", {" + q.ebnf(defs) + ", " + p.ebdnf() + "}";
        }, 0, p && many(discard(q) & p));
    }
}}
