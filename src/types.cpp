/*  This file is part of mastodonpp.
 *  Copyright © 2020, 2021 tastytea <tastytea@tastytea.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "types.hpp"

#include "log.hpp"

#include <algorithm>
#include <cctype>

namespace mastodonpp
{

using std::search;
using std::tolower;

answer_type::operator bool() const
{
    return (curl_error_code == 0 && http_status == 200);
}

answer_type::operator string_view() const
{
    return body;
}

std::ostream &operator<<(std::ostream &out, const answer_type &answer)
{
    out << answer.body;
    return out;
}

string_view answer_type::get_header(const string_view field) const
{
    const string_view searchstring{string(field) += ':'};
    // clang-format off
    auto it{search(headers.begin(), headers.end(), searchstring.begin(),
                   searchstring.end(), [](unsigned char a, unsigned char b)
                   { return tolower(a) == tolower(b); })};
    // clang-format on

    if (it != headers.end())
    {
        auto pos{static_cast<size_t>(it - headers.begin())};
        pos = headers.find(':', pos) + 2;
        const auto endpos{headers.find('\n', pos)};
        return string_view(&headers[pos], endpos - pos);
    }

    return {};
}

parametermap answer_type::parse_pagination(const bool next) const
{
    const string_view link{get_header("Link")};
    if (link.empty())
    {
        return {};
    }

    const string_view direction{next ? R"(rel="next")" : R"(rel="prev")"};
    auto endpos{link.find(direction)};
    endpos = link.rfind('>', endpos);
    auto startpos{link.rfind('?', endpos) + 1};
    const string_view paramstr{link.substr(startpos, endpos - startpos)};
    debuglog << "Found parameters in Link header: " << paramstr << '\n';

    startpos = 0;
    parametermap parameters;
    while ((endpos = paramstr.find('=', startpos)) != string_view::npos)
    {
        parameterpair param;
        param.first = paramstr.substr(startpos, endpos - startpos);
        startpos = endpos + 1;
        endpos = paramstr.find('&', startpos);
        param.second = paramstr.substr(startpos, endpos - startpos);
        parameters.insert(param);

        if (endpos == string_view::npos)
        {
            break;
        }
        startpos = endpos + 1;
    }

    return parameters;
}

} // namespace mastodonpp
