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

/*!
 *  @brief  Return type for Request%s.
 *
 *  @since  0.1.0
 *
 *  @headerfile return_types.hpp mastodonpp/return_types.hpp
 *
 */
struct answer_type
{
    /*!
     *  @brief  The error code returned by libcurl.
     *
     *  For more information consult
     *  [libcurl-errors(3)](https://curl.haxx.se/libcurl/c/libcurl-errors.html).
     *
     *  @since  0.1.0
     */
    uint8_t curl_error_code{0};

    /*!
     *  @brief  The error message.
     *
     *  @since  0.1.0
     */
    string error_message;

    /*!
     *  @brief  HTTP status code.
     *
     *  @since  0.1.0
     */
    uint16_t http_status{0};

    /*!
     *  @brief  The headers of the response from the server.
     *
     *  @since  0.1.0
     */
    string headers;

    /*!
     *  @brief  The response from the server, usually JSON.
     *
     *  @since  0.1.0
     */
    string body;

    /*!
     *  @brief  Returns true if #curl_error_code is 0 and #http_status is 200,
     *          false otherwise.
     *
     *  @since  0.1.0
     */
    explicit operator bool() const;

    /*!
     *  @brief  Returns #body as `std::string_view`.
     *
     *  @since  0.1.0
     */
    explicit operator string_view() const;

    /*!
     *  @brief  Returns #body as `std::ostream`.
     *
     *  @since  0.1.0
     */
    friend std::ostream &operator <<(std::ostream &out,
                                     const answer_type &answer);
};

} // namespace mastodonpp

#endif  // MASTODONPP_RETURN_TYPES_HPP