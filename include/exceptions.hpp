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

#ifndef MASTODONPP_EXCEPTIONS_HPP
#define MASTODONPP_EXCEPTIONS_HPP

#include <curl/curl.h>

#include <cstdint>
#include <exception>
#include <string>

namespace mastodonpp
{

using std::exception;
using std::string;
using std::uint16_t;

/*!
 *  @brief  Exception for libcurl errors.
 *
 *  @since  0.1.0
 *
 *  @headerfile exceptions.hpp mastodonpp/exceptions.hpp
 */
class CURLException : public exception
{
public:
    /*!
     *  @brief  Constructor with error code and message.
     *
     *  @since  0.1.0
     */
    explicit CURLException(const CURLcode &error, string message);

    /*!
     *  @brief Constructor with error code, message and error buffer.
     *
     *  @since  0.1.0
     */
    explicit CURLException(const CURLcode &error, string message,
                           string error_buffer);

    /*!
     *  @brief  Constructor with message.
     *
     *  @since  0.1.0
     */
    explicit CURLException(string message);

    /*!
     *  @brief  The error code returned by libcurl.
     *
     *  For more information consult
     *  [libcurl-errors(3)](https://curl.haxx.se/libcurl/c/libcurl-errors.html).
     *
     *  @since  0.1.0
     */
    const CURLcode error_code;

    /*!
     *  @brief  Returns the error code, message and error buffer.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] const char *what() const noexcept override;

private:
    const string _message;
    const string _error_buffer;
};

} // namespace mastodonpp

#endif // MASTODONPP_EXCEPTIONS_HPP
