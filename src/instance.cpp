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

#include "answer.hpp"
#include "instance.hpp"
#include "log.hpp"

#include <algorithm>
#include <exception>

namespace mastodonpp
{

using std::sort;
using std::stoull;
using std::exception;

Instance::Instance(const string_view hostname, const string_view access_token)
    : _hostname{hostname}
    , _baseuri{"https://" + _hostname}
    , _access_token{access_token}
    , _max_chars{0}
{}

uint64_t Instance::get_max_chars() noexcept
{
    constexpr uint64_t default_max_chars{500};

    if (_max_chars != 0)
    {
        return _max_chars;
    }

    try
    {
        debuglog << "Querying " << _hostname << " for max_toot_chars…\n";
        const auto answer{make_request(http_method::GET,
                                       _baseuri + "/api/v1/instance", {})};
        if (!answer)
        {
            debuglog << "Could not get instance info.\n";
            return default_max_chars;
        }

        _max_chars = [&answer]
        {
            auto &body{answer.body};
            size_t pos_start{body.find("max_toot_chars")};
            if (pos_start == string::npos)
            {
                debuglog << "max_toot_chars not found.\n";
                return default_max_chars;
            }
            pos_start = body.find(':', pos_start) + 1;
            const size_t pos_end{body.find(',', pos_start)};

            const auto max_toot_chars{body.substr(pos_start,
                                                  pos_end - pos_start)};
            return static_cast<uint64_t>(stoull(max_toot_chars));
        }();
        debuglog << "Set _max_chars to: " << _max_chars << '\n';
    }
    catch (const exception &e)
    {
        debuglog << "Unexpected exception: " << e.what() << '\n';
        return default_max_chars;
    }

    return _max_chars;
}

answer_type Instance::get_nodeinfo()
{
    debuglog << "Finding location of NodeInfo on " << _hostname << "…\n";
    auto answer{make_request(http_method::GET,
                             _baseuri + "/.well-known/nodeinfo", {})};
    if (!answer)
    {
        debuglog << "NodeInfo not found.\n";
        return answer;
    }

    size_t pos{0};
    vector<string> hrefs;
    constexpr string_view searchstring{R"("href":")"};
    while ((pos = answer.body.find(searchstring, pos)) != string::npos)
    {
        pos += searchstring.size();
        auto endpos{answer.body.find('"', pos)};
        hrefs.push_back(answer.body.substr(pos, endpos - pos));
        debuglog << "Found href: " << hrefs.back() << '\n';
    }
    sort(hrefs.begin(), hrefs.end()); // We assume they are sortable strings.
    debuglog << "Selecting href: " << hrefs.back() << '\n';

    return make_request(http_method::GET, hrefs.back(), {});
}

vector<string> Instance::get_post_formats() noexcept
{
    constexpr auto default_value{"text/plain"};

    if (!_post_formats.empty())
    {
        return _post_formats;
    }

    try
    {
        debuglog << "Querying " << _hostname << " for postFormats…\n";
        const auto answer{get_nodeinfo()};
        if (!answer)
        {
            debuglog << "Couldn't get NodeInfo.\n";
            _post_formats = {default_value};
            return _post_formats;
        }

        constexpr string_view searchstring{R"("postFormats":[)"};
        auto pos{answer.body.find(searchstring)};
        if (pos == string::npos)
        {
            debuglog << "Couldn't find metadata.postFormats.\n";
            _post_formats = {default_value};
            return _post_formats;
        }
        pos += searchstring.size();
        auto endpos{answer.body.find("],", pos)};
        string formats{answer.body.substr(pos, endpos - pos)};
        debuglog << "Extracted postFormats: " << formats << '\n';

        while ((pos = formats.find('"', 1)) != string::npos)
        {
            _post_formats.push_back(formats.substr(1, pos - 1));
            formats.erase(0, pos + 2); // 2 is the length of: ",
            debuglog << "Found postFormat: " << _post_formats.back() << '\n';
        }
    }
    catch (const std::exception &e)
    {
        debuglog << "Unexpected exception: " << e.what() << '\n';
        return {default_value};
    }

    return _post_formats;
}

    {
        debuglog << "Couldn't find metadata.postFormats.\n";
        _post_formats = {default_value};
        return _post_formats;
    }
    pos += searchstring.size();
    auto endpos{answer.body.find("],", pos)};
    string formats{answer.body.substr(pos, endpos - pos)};
    debuglog << "Extracted postFormats: " << formats << '\n';

    while ((pos = formats.find('"', 1)) != string::npos)
    {
        _post_formats.push_back(formats.substr(1, pos - 1));
        formats.erase(0, pos + 2); // 2 is the length of: ",
        debuglog << "Found postFormat: " << _post_formats.back() << '\n';
    }

    return _post_formats;
}

} // namespace mastodonpp
