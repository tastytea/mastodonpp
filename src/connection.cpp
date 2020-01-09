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

#include "connection.hpp"

namespace mastodonpp
{

using std::holds_alternative;

Connection::Connection(Instance &instance)
    : _instance{instance}
    , _baseuri{instance.get_baseuri()}
{
    auto proxy{_instance.get_proxy()};
    if (!proxy.empty())
    {
        CURLWrapper::set_proxy(proxy);
    }
}

answer_type Connection::get(const endpoint_variant &endpoint,
                            const parametermap &parameters)
{
    const string uri{[&]
    {
        if (holds_alternative<API::endpoint_type>(endpoint))
        {
            return string(_baseuri)
                += API{std::get<API::endpoint_type>(endpoint)}.to_string_view();
        }
        return string(std::get<string_view>(endpoint));
    }()};

    return make_request(http_method::GET, uri, parameters);
}

string Connection::get_new_stream_contents()
{
    buffer_mutex.lock();
    auto &buffer{get_buffer()};
    auto buffer_copy{buffer};
    buffer.clear();
    buffer_mutex.unlock();
    return buffer_copy;
}

vector<event_type> Connection::get_new_events()
{
    buffer_mutex.lock();
    auto &buffer{get_buffer()};
    vector<event_type> events;

    size_t pos{0};
    while ((pos = buffer.find("event: ")) != string::npos)
    {
        const auto endpos{buffer.find("\n\n", pos)};
        if (endpos == string::npos)
        {
            break;
        }

        event_type event;
        pos += 7;               // Length of "event: ".
        event.type = buffer.substr(pos, buffer.find('\n', pos) - pos);
        pos = buffer.find("data: ") + 6;
        event.data = buffer.substr(pos, endpos - pos);
        events.push_back(event);

        buffer.erase(0, endpos);
    }

    buffer_mutex.unlock();
    return events;
}

} // namespace mastodonpp
