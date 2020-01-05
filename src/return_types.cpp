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

#include "return_types.hpp"

namespace mastodonpp
{

answer_type::operator bool() const
{
    return (curl_error_code == 0 && http_status == 200);
}

answer_type::operator string_view() const
{
    return body;
}

std::ostream &operator <<(std::ostream &out, const answer_type &answer)
{
    out << answer.body;
    return out;
}

} // namespace mastodonpp
