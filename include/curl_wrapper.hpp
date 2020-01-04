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

#include "curl/curl.h"

#include <string>

namespace mastodonpp
{

using std::string;

//! Internal use only.
extern bool curl_initialized;

/*!
 *  @brief  Handles the details of network connections.
 *
 *  @since  0.1.0
 *
 *  @headerfile curl_wrapper.hpp mastodonpp/curl_wrapper.hpp
 */
class CURLWrapper
{
public:
    /*!
     *  @brief  Constructs a CURLWrapper object.
     *
     *  The first construction of an CurlWrapper object will call
     *  `curl_global_init`, which is not thread-safe. For more information
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

    //! Destructor
    virtual ~CURLWrapper() noexcept;

    //! Copy assignment operator
    CURLWrapper& operator=(const CURLWrapper &other) = default;

    //! Move assignment operator
    CURLWrapper& operator=(CURLWrapper &&other) noexcept = default;

private:
    CURL *_connection;
    char _curl_buffer_error[CURL_ERROR_SIZE];
    string _curl_buffer;

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
