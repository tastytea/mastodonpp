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

Connection::Connection(Instance &instance)
    : _instance{instance}
    , _baseuri{instance.get_baseuri()}
{}

answer_type Connection::get(const API::endpoint_type &endpoint)
{
    return make_request(
        http_method::GET,
        string(_baseuri).append(API{endpoint}.to_string_view()));
}

answer_type Connection::get(const string_view &endpoint)
{
    return make_request(http_method::GET, string(_baseuri).append(endpoint));
}

} // namespace mastodonpp
