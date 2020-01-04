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

#include "api.hpp"

#include <map>
#include <string_view>
#include <utility>

namespace mastodonpp
{

using std::map;
using std::string_view;
using std::move;

API::API(endpoint_type &endpoint)
    : _endpoint{move(endpoint)}
{}

string API::to_string() const
{
    static const map<endpoint_type,string_view> endpoint_map
        {
            {v1::instance, "/api/v1/instance"},
            {v2::search, "/api/v2/search"}
        };
    return endpoint_map.at(_endpoint).data();
}
} // namespace mastodonpp
