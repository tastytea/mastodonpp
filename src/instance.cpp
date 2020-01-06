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

namespace mastodonpp
{

using std::stoull;

Instance::Instance(const string_view &hostname, const string_view &access_token)
    : _hostname{hostname}
    , _baseuri{"https://" + _hostname}
    , _access_token{access_token}
    , _max_chars{500}
{
    try
    {
        debuglog << "Querying " << _hostname << " for max_toot_chars…\n";
        const auto answer{make_request(http_method::GET,
                                       _baseuri + "/api/v1/instance")};
        if (!answer)
        {
            return;
        }

        _max_chars = [&answer]
        {
            auto &body{answer.body};
            size_t pos_start{body.find("max_toot_chars")};
            if (pos_start == string::npos)
            {
                debuglog << "max_toot_chars not found.\n";
                return static_cast<uint64_t>(500);
            }
            pos_start = body.find(':', pos_start) + 1;
            const size_t pos_end{body.find(',', pos_start)};

            const auto max_toot_chars{body.substr(pos_start,
                                                  pos_end - pos_start)};
            return static_cast<uint64_t>(stoull(max_toot_chars));
        }();
        debuglog << "Set _max_chars to: " << _max_chars << '\n';
    }
    catch (const std::exception &e)
    {
        debuglog << "Unexpected exception: " << e.what() << '\n';
    }
}

} // namespace mastodonpp
