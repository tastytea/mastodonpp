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

#ifndef MASTODONPP_CURL_WRAPPER_HPP
#define MASTODONPP_CURL_WRAPPER_HPP

#include "return_types.hpp"

#include "curl/curl.h"

#include <string>
#include <string_view>

namespace mastodonpp
{

using std::string;
using std::string_view;

/*!
 *  @brief  The HTTP method.
 *
 *  @since  0.1.0
 */
enum class http_method
{
    GET,
    POST,
    PATCH,
    DELETE
};

/*!
 *  @brief  Handles the details of network connections.
 *
 *  You don't need to use this.
 *
 *  @since  0.1.0
 *
 *  @headerfile curl_wrapper.hpp mastodonpp/curl_wrapper.hpp
 */
class CURLWrapper
{
public:
    /*!
     *  @brief  Initializes curl and sets up connection.
     *
     *  Calls `curl_global_init`, which is not thread-safe. For more information
     *  consult [curl_global_init(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_global_init.html).
     *
     *  @since  0.1.0
     */
    CURLWrapper();

    //! Copy constructor
    CURLWrapper(const CURLWrapper &other) = default;

    //! Move constructor
    CURLWrapper(CURLWrapper &&other) noexcept = default;

    /*!
     *  @brief  Cleans up curl and connection.
     *
     *  Calls `curl_global_cleanup`, which is not thread-safe. For more
     *  information consult [curl_global_cleanup(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_global_cleanup.html).
     *
     *  @since  0.1.0
     */
    virtual ~CURLWrapper() noexcept;

    //! Copy assignment operator
    CURLWrapper& operator=(const CURLWrapper &other) = default;

    //! Move assignment operator
    CURLWrapper& operator=(CURLWrapper &&other) noexcept = default;

    /*!
     *  @brief  Make a request.
     *
     *  @param  method The HTTP method.
     *  @param  uri    The full URI.
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    answer_type make_request(const http_method &method, const string_view &uri);

private:
    CURL *_connection;
    char _curl_buffer_error[CURL_ERROR_SIZE];
    string _curl_buffer_headers;
    string _curl_buffer_body;

    /*!
     *  @brief  libcurl write callback function.
     *
     *  @since  0.1.0
     */
    static int writer(char *data, size_t size, size_t nmemb,
                      string *writerData);

    /*!
     *  @brief  Setup libcurl connection.
     *
     *  @since  0.1.0
     */
    void setup_curl();
};

} // namespace mastodonpp

#endif  // MASTODONPP_CURL_WRAPPER_HPP
