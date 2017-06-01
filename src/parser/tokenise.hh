#pragma once

#include "parser/rename.hh"
#include "predicate/predicates.hh"
#include "combinator/operators.hh"

namespace Parsing {
namespace parser
{
    //----------------------------------------------------------------------------
    // Lazy Tokenisation, by skipping the whitespace after each token we leave
    // the input stream in a position where parsers can fail on the first
    // character without requiring backtracking. This requires using the
    // "first_token" parser as the first parser to initialise lazy tokenization
    // properly.

    // skip to start of first token.

    template <typename R> constexpr auto tokenise(R const& r)
    {
        using predicate::space;

        constexpr auto first_token = discard(many(accept(space<char>)));

        return rename([r] (dictionary_t *defs)
        {
            return r.ebnf(defs);
        }, 0, r && first_token);
    }
}}
