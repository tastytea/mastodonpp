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
#include "types.hpp"

#include <mutex>
#include <string>
#include <string_view>

namespace mastodonpp
{

using std::mutex;
using std::string;
using std::string_view;

/*!
 *  @brief  The HTTP method.
 *
 *  @since  0.1.0
 */
enum class http_method
{
    GET,   // NOLINT(readability-identifier-naming)
    POST,  // NOLINT(readability-identifier-naming)
    PATCH, // NOLINT(readability-identifier-naming)
    PUT,   // NOLINT(readability-identifier-naming)
    DELETE // NOLINT(readability-identifier-naming)
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
     *  The first time an instance of CURLWrapper is created, it calls
     *  `curl_global_init`, which is not thread-safe. For more information
     *  consult [curl_global_init(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_global_init.html).
     *
     *  @since  0.1.0
     */
    CURLWrapper();

    /*!
     *  @brief  Copy constructor. Does the same as the Constructor.
     *
     *  @since  0.5.2
     */
    CURLWrapper(const CURLWrapper &);

    //! Move constructor
    CURLWrapper(CURLWrapper &&other) noexcept = delete;

    /*!
     *  @brief  Cleans up curl and connection.
     *
     *  May call `curl_global_cleanup`, which is not thread-safe. For more
     *  information consult [curl_global_cleanup(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_global_cleanup.html).
     *
     *  @since  0.1.0
     */
    virtual ~CURLWrapper() noexcept;

    //! Copy assignment operator
    CURLWrapper &operator=(const CURLWrapper &other) = delete;

    //! Move assignment operator
    CURLWrapper &operator=(CURLWrapper &&other) noexcept = delete;

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
     *  @brief  URL encodes the given string.
     *
     *  For more information consult [curl_easy_escape(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_easy_escape.html).
     *
     *  @param  url String to escape.
     *
     *  @return The escaped string or {} if it failed.
     *
     *  @since  0.3.0
     */
    [[nodiscard]] inline string escape_url(const string_view url) const
    {
        char *cbuf{curl_easy_escape(_connection, url.data(),
                                    static_cast<int>(url.size()))};
        string sbuf{cbuf};
        curl_free(cbuf);
        return sbuf;
    }

    /*!
     *  @brief  URL decodes the given string.
     *
     *  For more information consult [curl_easy_unescape(3)]
     *  (https://curl.haxx.se/libcurl/c/curl_easy_unescape.html).
     *
     *  @param  url String to unescape.
     *
     *  @return The unescaped string or {} if it failed.
     *
     *  @since  0.3.0
     */
    [[nodiscard]] inline string unescape_url(const string_view url) const
    {
        char *cbuf{curl_easy_unescape(_connection, url.data(),
                                      static_cast<int>(url.size()), nullptr)};
        string sbuf{cbuf};
        curl_free(cbuf);
        return sbuf;
    }

    /*!
     *  @brief  Set some properties of the connection.
     *
     *  Meant for internal use. See Instance::copy_connection_properties().
     *
     *  @since  0.3.0
     */
    void setup_connection_properties(string_view proxy,
                                     string_view access_token,
                                     string_view cainfo, string_view useragent);

protected:
    /*!
     *  @brief  Mutex for #get_buffer a.k.a. _curl_buffer_body.
     *
     *  This mutex is locked before anything is read or written from/to
     *  _curl_buffer_body.
     *
     *  @since  0.1.0
     */
    mutex _buffer_mutex;

    /*!
     *  @brief  Make a HTTP request.
     *
     *  @param  method     The HTTP method.
     *  @param  uri        The full URI.
     *  @param  parameters A map of parameters.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] answer_type make_request(const http_method &method,
                                           string uri,
                                           const parametermap &parameters);

    /*!
     *  @brief  Returns a reference to the buffer libcurl writes into.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] inline string &get_buffer()
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
     *  @brief  Set the proxy to use.
     *
     *  See [CURLOPT_PROXY(3)]
     *  (https://curl.haxx.se/libcurl/c/CURLOPT_PROXY.html).
     *
     *  @param  proxy Examples: "socks4a://127.0.0.1:9050", "http://[::1]:3128".
     *
     *  @since  0.1.0
     */
    virtual void set_proxy(string_view proxy);

    /*!
     *  @brief  Set OAuth 2.0 Bearer Access Token.
     *
     *  @since  0.1.0
     */
    void set_access_token(string_view access_token);

    /*!
     *  @brief  Set path to Certificate Authority (CA) bundle.
     *
     *  @since  0.3.0
     */
    virtual void set_cainfo(string_view path);

    /*!
     *  @brief  Sets the User-Agent.
     *
     *  @since  0.3.0
     */
    virtual void set_useragent(string_view useragent);

private:
    CURL *_connection{nullptr};
    char _curl_buffer_error[CURL_ERROR_SIZE]{'\0'};
    string _curl_buffer_headers;
    string _curl_buffer_body;
    bool _stream_cancelled{false};

    /*!
     *  @brief  Initializes curl and sets up connection.
     *
     *  @since  0.5.2
     */
    void init();

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
        return static_cast<CURLWrapper *>(f)->writer_body(data, sz, nmemb);
    }

    //! @copydoc writer_body
    size_t writer_header(char *data, size_t size, size_t nmemb);

    //! @copydoc writer_body_wrapper
    static inline size_t writer_header_wrapper(char *data, size_t sz,
                                               size_t nmemb, void *f)
    {
        return static_cast<CURLWrapper *>(f)->writer_header(data, sz, nmemb);
    }

    /*!
     *  @brief  libcurl transfer info function.
     *
     *  Used to cancel streams.
     *
     *  @since  0.1.0
     */
    int progress(void *clientp, curl_off_t dltotal, curl_off_t dlnow,
                 curl_off_t ultotal, curl_off_t ulnow) const;

    //! @copydoc writer_body_wrapper
    static inline int progress_wrapper(void *f, void *clientp,
                                       curl_off_t dltotal, curl_off_t dlnow,
                                       curl_off_t ultotal, curl_off_t ulnow)
    {
        return static_cast<CURLWrapper *>(f)->progress(clientp, dltotal, dlnow,
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
    static bool replace_parameter_in_uri(string &uri,
                                         const parameterpair &parameter);

    /*!
     *  @brief  Add parameters to URI.
     *
     *  @param  uri        Reference to the URI.
     *  @param  parameters The parametermap.
     *
     *  @since  0.1.0
     */
    static void add_parameters_to_uri(string &uri,
                                      const parametermap &parameters);

    /*!
     *  @brief  Add `*curl_mimepart` to `*curl_mime`.
     *
     *  @param  mime Initialized `*curl_mime`.  @param name Name of the field.
     *  @param  data Data of the field. If it begins with <tt>`\@file:<tt>, the
     *               rest of the ergument is treated as a filename.
     *
     *  @since  0.2.0
     */
    static void add_mime_part(curl_mime *mime, string_view name,
                              string_view data);

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

#endif // MASTODONPP_CURL_WRAPPER_HPP
