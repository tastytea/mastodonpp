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

string Connection::endpoint_to_uri(const endpoint_variant &endpoint) const
{
    if (holds_alternative<API::endpoint_type>(endpoint))
    {
        return string(_baseuri) += API{std::get<API::endpoint_type>(endpoint)}
                                       .to_string_view();
    }
    return string(_baseuri) += std::get<string_view>(endpoint);
}

answer_type Connection::get(const endpoint_variant &endpoint,
                            const parametermap &parameters)
{
    return make_request(http_method::GET, endpoint_to_uri(endpoint),
                        parameters);
}

answer_type Connection::post(const endpoint_variant &endpoint,
                             const parametermap &parameters)
{
    return make_request(http_method::POST, endpoint_to_uri(endpoint),
                        parameters);
}

answer_type Connection::patch(const endpoint_variant &endpoint,
                              const parametermap &parameters)
{
    return make_request(http_method::PATCH, endpoint_to_uri(endpoint),
                        parameters);
}

answer_type Connection::put(const endpoint_variant &endpoint,
                            const parametermap &parameters)
{
    return make_request(http_method::PUT, endpoint_to_uri(endpoint),
                        parameters);
}

answer_type Connection::del(const endpoint_variant &endpoint,
                            const parametermap &parameters)
{
    return make_request(http_method::DELETE, endpoint_to_uri(endpoint),
                        parameters);
}

string Connection::get_new_stream_contents()
{
    _buffer_mutex.lock();
    auto &buffer{get_buffer()};
    string buffer_copy{buffer};
    buffer.clear();
    _buffer_mutex.unlock();
    return buffer_copy;
}

vector<event_type> Connection::get_new_events()
{
    _buffer_mutex.lock();
    auto &buffer{get_buffer()};
    vector<event_type> events;

    size_t pos{0};
    constexpr string_view search_event{"event: "};
    while ((pos = buffer.find(search_event)) != string::npos)
    {
        const auto endpos{buffer.find("\n\n", pos)};
        if (endpos == string::npos)
        {
            break;
        }

        event_type event;
        pos += search_event.size();
        event.type = buffer.substr(pos, buffer.find('\n', pos) - pos);
        constexpr string_view search_data{"data: "};
        pos = buffer.find(search_data) + search_data.size();
        event.data = buffer.substr(pos, endpos - pos);
        events.push_back(event);

        buffer.erase(0, endpos);
    }

    _buffer_mutex.unlock();
    return events;
}

} // namespace mastodonpp
