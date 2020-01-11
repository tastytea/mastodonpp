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
#include <mutex>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace mastodonpp
{

using std::map;
using std::mutex;
using std::string;
using std::string_view;
using std::pair;
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
using parametermap =
    map<string_view, variant<string_view, vector<string_view>>>;

/*!
 *  @brief  A single parameter of a parametermap.
 *
 *  @since  0.1.0
 */
using parameterpair =
    pair<string_view, variant<string_view, vector<string_view>>>;

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
    CURLWrapper(const CURLWrapper &other) = delete;

    //! Move constructor
    CURLWrapper(CURLWrapper &&other) noexcept = delete;

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
    CURLWrapper& operator=(const CURLWrapper &other) = delete;

    //! Move assignment operator
    CURLWrapper& operator=(CURLWrapper &&other) noexcept = delete;

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
     *  @brief  Mutex for #get_buffer a.k.a. _curl_buffer_body.
     *
     *  This mutex is locked before anything is read or written from/to
     *  _curl_buffer_body.
     *
     *  @since  0.1.0
     */
    mutex buffer_mutex;

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

    /*!
     *  @brief  Returns a reference to the buffer libcurl writes into.
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    string &get_buffer()
    {
        return _curl_buffer_body;
    }

    /*!
     *  @brief  Cancel the stream.
     *
     *  The stream will be cancelled, usually whithin a second. The @link
     *  answer_type::curl_error_code curl_error_code @endlink of the answer will
     *  be set to 42 (`CURLE_ABORTED_BY_CALLBACK`).
     *
     *  @since  0.1.0
     */
    inline void cancel_stream()
    {
        _stream_cancelled = true;
    }

    /*!
     *  @brief  Set OAuth 2.0 Bearer Access Token.
     *
     *  @since  0.1.0
     */
    void set_access_token(const string_view access_token);

private:
    CURL *_connection;
    char _curl_buffer_error[CURL_ERROR_SIZE];
    string _curl_buffer_headers;
    string _curl_buffer_body;
    bool _stream_cancelled;

    /*!
     *  @brief  libcurl write callback function.
     *
     *  @since  0.1.0
     */
    size_t writer_body(char *data, size_t size, size_t nmemb);

    /*!
     *  @brief  Wrapper for curl, because it can only call static member
     *          functions.
     *
     *  <https://curl.haxx.se/docs/faq.html#Using_C_non_static_functions_f>
     *
     *  @since  0.1.0
     */
    static inline size_t writer_body_wrapper(char *data, size_t sz,
                                             size_t nmemb, void *f)
    {
        return static_cast<CURLWrapper*>(f)->writer_body(data, sz, nmemb);
    }

    //! @copydoc writer_body
    size_t writer_header(char *data, size_t size, size_t nmemb);

    //! @copydoc writer_body_wrapper
    static inline size_t writer_header_wrapper(char *data, size_t sz,
                                               size_t nmemb, void *f)
    {
        return static_cast<CURLWrapper*>(f)->writer_header(data, sz, nmemb);
    }

    /*!
     *  @brief  libcurl transfer info function.
     *
     *  Used to cancel streams.
     *
     *  @since  0.1.0
     */
    int progress(void *clientp, curl_off_t dltotal, curl_off_t dlnow,
                 curl_off_t ultotal, curl_off_t ulnow);

    //! @copydoc writer_body_wrapper
    static inline int progress_wrapper(void *f, void *clientp,
                                       curl_off_t dltotal, curl_off_t dlnow,
                                       curl_off_t ultotal, curl_off_t ulnow)
    {
        return static_cast<CURLWrapper*>(f)->progress(clientp, dltotal, dlnow,
                                                      ultotal, ulnow);
    }

    /*!
     *  @brief  Setup libcurl connection.
     *
     *  @since  0.1.0
     */
    void setup_curl();

    /*!
     *  @brief  Replace parameter in URI.
     *
     *  @param  uri       Reference to the URI.
     *  @param  parameter The parameter.
     *
     *  @return true if parameter was replaced.
     *
     *  @since  0.1.0
     */
    bool replace_parameter_in_uri(string &uri, const parameterpair &parameter);

    /*!
     *  @brief  Add parameters to URI.
     *
     *  @param  uri        Reference to the URI.
     *  @param  parameters The parametermap.
     *
     *  @since  0.1.0
     */
    void add_parameters_to_uri(string &uri, const parametermap &parameters);

    /*!
     *  @brief  Add `*curl_mimepart` to `*curl_mime`.
     *
     *  @param  mime Initialized `*curl_mime`.  @param name Name of the field.
     *  @param  data Data of the field. If it begins with <tt>`\@file:<tt>, the
     *               rest of the ergument is treated as a filename.
     *
     *  @since  0.1.1
     */
    void add_mime_part(curl_mime *mime,
                       string_view name, string_view data) const;

    /*!
     *  @brief  Convert parametermap to `*curl_mime`.
     *
     *  For more information consult [curl_mime_init(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_mime_init.html). Calls
     *  replace_parameter_in_uri().
     *
     *  @param  uri        Reference to the URI.
     *  @param  parameters The parametermap.
     *
     *  @return `*curl_mime`.
     *
     *  @since  0.1.0
     */
    curl_mime *parameters_to_curl_mime(string &uri,
                                       const parametermap &parameters);
};

} // namespace mastodonpp

#endif  // MASTODONPP_CURL_WRAPPER_HPP
