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

#include "answer.hpp"

#include "curl/curl.h"

#include <map>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace mastodonpp
{

using std::map;
using std::string;
using std::string_view;
using std::variant;
using std::vector;

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
    PUT,
    DELETE
};

/*!
 *  @brief  std::map of parameters for API calls.
 *
 *  Example:
 *  @code
 *  parametermap parameters
 *      {
 *          {"id", "12"},
 *          {"poll[options]", vector<string_view>{"Yes", "No", "Maybe"}}
 *      };
 *  @endcode
 *
 *  @since  0.1.0
 */
using parametermap = map<string_view, variant<string_view, vector<string_view>>>;

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
     *  The first time an instance of CURLWrapper is created, it calls
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
     *  @brief  Returns pointer to the CURL easy handle.
     *
     *  You can use this handle to set or modify curl options. For more
     *  information consult [curl_easy_setopt(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_easy_setopt.html).
     *
     *  @since  0.1.0
     */
    inline CURL *get_curl_easy_handle()
    {
        return _connection;
    }

    /*!
     *  @brief  Set the proxy to use.
     *
     *  See [CURLOPT_PROXY(3)]
     *  (https://curl.haxx.se/libcurl/c/CURLOPT_PROXY.html).
     *
     *  @param  proxy Examples: "socks4a://127.0.0.1:9050", "http://[::1]:3128".
     *
     *  @since  0.1.0
     */
    void set_proxy(string_view proxy);

protected:
    /*!
     *  @brief  Make a HTTP request.
     *
     *  @param  method     The HTTP method.
     *  @param  uri        The full URI.
     *  @param  parameters A map of parameters.
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    answer_type make_request(const http_method &method, string uri,
                             const parametermap &parameters);

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

    /*!
     *  @brief  Add parameters to URI.
     *
     *  @param  uri        Reference to the URI.
     *  @param  parameters The parametermap.
     *
     *  @since  0.1.0
     */
    void add_parameters_to_uri(string &uri, const parametermap &parameters);
};

} // namespace mastodonpp

#endif  // MASTODONPP_CURL_WRAPPER_HPP
