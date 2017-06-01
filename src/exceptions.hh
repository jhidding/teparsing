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

#include <stdexcept>
#include <string>
#include <sstream>
#include <map>

namespace Parsing
{
    using dictionary_t = std::map<std::string, std::string>;

    template <typename Parser, typename Iterator, typename Range>
    std::string error_message(
            std::string const &what, Parser const &p,
            Iterator const &f, Iterator const &l, Range const &r)
    {
        std::ostringstream err;

        Iterator i(r.first);
        Iterator line_start(r.first);
        int row = 1;
        while ((i != r.end()) && (i != f)) {
            if (*i == '\n') {
                ++row;
                line_start = ++i;
            } else {
                ++i;
            }
        }

        err << what << " at line: " << row
            << " column: " << f - line_start + 1 << std::endl;

        bool in = true;
        for (Iterator i(line_start); (i != r.end()) && (in || *i != '\n'); ++i) {
            if (i == l) {
                in = false;
            }
            if (::isspace(*i)) {
                err << ' ';
            } else {
                err << static_cast<char>(*i);
            }
        }
        err << std::endl;

        i = line_start;
        while (i != f) {
            err << ' ';
            ++i;
        }

        err << '^';
        ++i;

        if (i != l) {
            ++i;
            while (i != l) {
                err << '-';
                ++i;
            }
            err << "^";
        }

        err << std::endl << "expecting: ";

        dictionary_t defs;
        err << p.ebnf(&defs) << std::endl << "where:" << std::endl;

        for (auto const& d : defs)
        {
            err << "\t" << d.first << " = " << d.second << ";" << std::endl;
        }

        return err.str();
    }

    class error: public std::runtime_error
    {
        public:
            template <typename Parser, typename Iterator, typename Range>
            error(std::string const& what, Parser const& p,
                  Iterator const &f, Iterator const &l, Range const &r)
                : runtime_error(error_message(what, p, f, l, r))
            {}
    };
}
