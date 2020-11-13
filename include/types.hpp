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

// Types that are used in more than one file.

#ifndef MASTODONPP_TYPES_HPP
#define MASTODONPP_TYPES_HPP

#include <cstdint>
#include <map>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace mastodonpp
{

using std::map;
using std::ostream;
using std::pair;
using std::string;
using std::string_view;
using std::uint16_t;
using std::uint8_t;
using std::variant;
using std::vector;

/*!
 *  @brief  `std::map` of parameters for %API calls.
 *
 *  Note that arrays always have to be specified as vectors, even if they have
 *  only 1 element. To send a file, use “<tt>\@file:</tt>” followed by the file
 *  name as value.
 *
 *  Example:
 *  @code
 *  parametermap parameters
 *      {
 *          {"poll[expires_in]", "86400"},
 *          {"poll[options]", vector<string_view>{"Yes", "No", "Maybe"}},
 *          {"status", "How is the weather?"}
 *      };
 *  @endcode
 *
 *  @since  0.1.0
 */
using parametermap = map<string_view,
                         variant<string_view, vector<string_view>>>;

/*!
 *  @brief  A single parameter of a parametermap.
 *
 *  @since  0.1.0
 */
using parameterpair = pair<string_view,
                           variant<string_view, vector<string_view>>>;

/*!
 *  @brief  Return type for Request%s.
 *
 *  @since  0.1.0
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
     *
     *  @headerfile types.hpp mastodonpp/types.hpp
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
    friend ostream &operator<<(ostream &out, const answer_type &answer);

    /*!
     *  @brief  Returns the value of a header field.
     *
     *  Is only valid for as long as the answer_type is in scope.
     *
     *  @param  field Case insensitive, only ASCII.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] string_view get_header(string_view field) const;

    /*!
     *  @brief  Returns the parameters needed for the next entries.
     *
     *  Parses the `Link` header.
     *
     *  @since  0.3.0
     */
    [[nodiscard]] inline parametermap next() const
    {
        return parse_pagination(true);
    }

    /*!
     *  @brief  Returns the parameters needed for the previous entries.
     *
     *
     *  Parses the `Link` header.
     *
     *  @since  0.3.0
     */
    [[nodiscard]] inline parametermap prev() const
    {
        return parse_pagination(false);
    }

private:
    /*!
     *  @brief  Returns the parameters needed for the next or previous entries.
     *
     *
     *  Parses the `Link` header.
     *
     *  @since  0.3.0
     */
    [[nodiscard]] parametermap parse_pagination(bool next) const;
};

} // namespace mastodonpp

#endif // MASTODONPP_TYPES_HPP
