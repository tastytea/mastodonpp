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

#ifndef MASTODONPP_RETURN_TYPES_HPP
#define MASTODONPP_RETURN_TYPES_HPP

#include <cstdint>
#include <string>
#include <string_view>

namespace mastodonpp
{

using std::uint8_t;
using std::uint16_t;
using std::string;
using std::string_view;

struct answer
{
    uint8_t error_code;
    string error_message;
    uint16_t http_status;
    string body;

    explicit operator bool() const;
    explicit operator string_view() const;
    friend std::ostream &operator <<(std::ostream &out, const answer &answer);
};

} // namespace mastodonpp

#endif  // MASTODONPP_RETURN_TYPES_HPP
