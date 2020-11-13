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

#include "exceptions.hpp"

#include <utility>

namespace mastodonpp
{

using std::move;
using std::to_string;

CURLException::CURLException(const CURLcode &error, string message)
    : error_code{error}
    , _message{move(message)}
{}

CURLException::CURLException(const CURLcode &error, string message,
                             string error_buffer)
    : error_code{error}
    , _message{move(message)}
    , _error_buffer{move(error_buffer)}
{}

CURLException::CURLException(string message)
    : error_code{CURLE_OK}
    , _message{move(message)}
{}

const char *CURLException::what() const noexcept
{
    static string error_string{"libCURL error: "};
    if (error_code != CURLE_OK)
    {
        error_string += to_string(error_code) + " - ";
    }
    error_string += _message;
    if (!_error_buffer.empty())
    {
        error_string += " [" + _error_buffer + "]";
    }
    return error_string.c_str();
}

} // namespace mastodonpp
