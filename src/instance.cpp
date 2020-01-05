/*  This file is part of mastodonpp.
 *  Copyright © 2020 tastytea <tastytea@tastytea.de>
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

#include "instance.hpp"
#include "log.hpp"
#include "return_types.hpp"

#include <utility>

namespace mastodonpp
{

using std::move;

Instance::Instance(string hostname, string access_token)
    : _hostname{move(hostname)}
    , _baseuri{"https://" + _hostname}
    , _access_token{move(access_token)}
    , _max_chars{500}
{
    try
    {
        const auto answer{make_request(http_method::GET,
                                       _baseuri + "/api/v1/instance")};
        if (answer)
        {
            debuglog << "Querying instance for max_toot_chars…\n";
            auto &body{answer.body};
            size_t pos_start{body.find("max_toot_chars")};
            if (pos_start == string::npos)
            {
                debuglog << "max_toot_chars not found.";
                return;
            }
            pos_start = body.find(':', pos_start) + 1;
            const size_t pos_end{body.find(',', pos_start)};

            const auto max_toot_chars{body.substr(pos_start,
                                                  pos_end - pos_start)};
            _max_chars = std::stoull(max_toot_chars);
            debuglog << "Set _max_chars to: " << _max_chars << '\n';
        }
    }
    catch (const std::exception &e)
    {
        debuglog << "Unexpected exception: " << e.what() << '\n';
    }
}

} // namespace mastodonpp
