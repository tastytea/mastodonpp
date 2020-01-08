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
{}

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

void Connection::set_proxy(const string_view proxy)
{
    CURLWrapper::set_proxy(proxy);
    _instance.set_proxy(proxy);
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

} // namespace mastodonpp
