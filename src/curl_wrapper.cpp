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

#include "curl_wrapper.hpp"

#include "exceptions.hpp"
#include "log.hpp"
#include "version.hpp"

#include <algorithm>
#include <array>
#include <atomic>
#include <cctype>
#include <cstdint>

namespace mastodonpp
{

using std::any_of;
using std::array; // NOLINT(misc-unused-using-decls)
using std::atomic;
using std::get;
using std::holds_alternative;
using std::toupper;
using std::transform;
using std::uint16_t;
using std::uint8_t;

// No one will ever need more than 65535 connections. 😉
static atomic<uint16_t> curlwrapper_instances{0};

void CURLWrapper::init()
{
    if (curlwrapper_instances == 0)
    {
        curl_global_init(CURL_GLOBAL_ALL); // NOLINT(hicpp-signed-bitwise)
    }
    ++curlwrapper_instances;
    debuglog << "CURLWrapper instances: " << curlwrapper_instances << " (+1)\n";

    _connection = curl_easy_init();
    setup_curl();
}

CURLWrapper::CURLWrapper()
{
    init();
}

CURLWrapper::CURLWrapper(const CURLWrapper &)
{
    init();
}

CURLWrapper::~CURLWrapper() noexcept
{
    curl_easy_cleanup(_connection);

    --curlwrapper_instances;
    debuglog << "CURLWrapper instances: " << curlwrapper_instances << " (-1)\n";
    if (curlwrapper_instances == 0)
    {
        curl_global_cleanup();
    }
}

answer_type CURLWrapper::make_request(const http_method &method, string uri,
                                      const parametermap &parameters)
{
    _stream_cancelled = false;
    _curl_buffer_headers.clear();
    _curl_buffer_body.clear();

    CURLcode code{CURLE_OK};
    switch (method)
    {
    case http_method::GET:
    {
        add_parameters_to_uri(uri, parameters);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(_connection, CURLOPT_HTTPGET, 1L);

        break;
    }
    case http_method::POST:
    {
        if (parameters.empty())
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
            curl_easy_setopt(_connection, CURLOPT_POST, 1L);
        }
        else
        {
            curl_mime *mime{parameters_to_curl_mime(uri, parameters)};
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
            curl_easy_setopt(_connection, CURLOPT_MIMEPOST, mime);
        }

        break;
    }
    case http_method::PATCH:
    {
        if (!parameters.empty())
        {
            curl_mime *mime{parameters_to_curl_mime(uri, parameters)};
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
            curl_easy_setopt(_connection, CURLOPT_MIMEPOST, mime);
        }

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        code = curl_easy_setopt(_connection, CURLOPT_CUSTOMREQUEST, "PATCH");
        if (code != CURLE_OK)
        {
            throw CURLException{code, "Failed to set URI", _curl_buffer_error};
        }

        break;
    }
    case http_method::PUT:
    {
        if (!parameters.empty())
        {
            curl_mime *mime{parameters_to_curl_mime(uri, parameters)};
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
            curl_easy_setopt(_connection, CURLOPT_MIMEPOST, mime);
        }

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        code = curl_easy_setopt(_connection, CURLOPT_CUSTOMREQUEST, "PUT");
        if (code != CURLE_OK)
        {
            throw CURLException{code, "Failed to set URI", _curl_buffer_error};
        }

        break;
    }
    case http_method::DELETE:
    {
        if (!parameters.empty())
        {
            curl_mime *mime{parameters_to_curl_mime(uri, parameters)};
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
            curl_easy_setopt(_connection, CURLOPT_MIMEPOST, mime);
        }

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        code = curl_easy_setopt(_connection, CURLOPT_CUSTOMREQUEST, "DELETE");
        if (code != CURLE_OK)
        {
            throw CURLException{code, "Failed to set URI", _curl_buffer_error};
        }

        break;
    }
    }
    debuglog << "Making request to: " << uri << '\n';

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    code = curl_easy_setopt(_connection, CURLOPT_URL, uri.data());
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set URI", _curl_buffer_error};
    }

    answer_type answer;
    code = curl_easy_perform(_connection);
    if (code == CURLE_OK
        || (code == CURLE_ABORTED_BY_CALLBACK && _stream_cancelled))
    {
        long http_status{0}; // NOLINT(google-runtime-int)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_getinfo(_connection, CURLINFO_RESPONSE_CODE, &http_status);
        answer.http_status = static_cast<uint16_t>(http_status);
        debuglog << "HTTP status code: " << http_status << '\n';

        answer.headers = _curl_buffer_headers;
        answer.body = _curl_buffer_body;
    }
    else
    {
        answer.curl_error_code = static_cast<uint8_t>(code);
        answer.error_message = _curl_buffer_error;
        debuglog << "libcurl error: " << code << '\n';
        debuglog << _curl_buffer_error << '\n';
    }

    return answer;
}

void CURLWrapper::setup_connection_properties(const string_view proxy,
                                              const string_view access_token,
                                              const string_view cainfo,
                                              const string_view useragent)
{
    if (!proxy.empty())
    {
        set_proxy(proxy);
    }

    if (!access_token.empty())
    {
        set_access_token(access_token);
    }

    if (!cainfo.empty())
    {
        set_cainfo(cainfo);
    }

    if (!useragent.empty())
    {
        set_useragent(useragent);
    }
}

void CURLWrapper::set_proxy(const string_view proxy)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    CURLcode code{curl_easy_setopt(_connection, CURLOPT_PROXY, proxy.data())};
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set proxy", _curl_buffer_error};
    }
    debuglog << "Set proxy to: " << proxy << '\n';
}

void CURLWrapper::set_access_token(const string_view access_token)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg, hicpp-signed-bitwise)
    CURLcode code{curl_easy_setopt(_connection, CURLOPT_XOAUTH2_BEARER,
                                   access_token.data())};
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Could not set authorization token.",
                            _curl_buffer_error};
    }

#if (LIBCURL_VERSION_NUM < 0x073d00) // libcurl < 7.61.0.
#    define CURLAUTH_BEARER CURLAUTH_ANY
#endif

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg, hicpp-signed-bitwise)
    code = curl_easy_setopt(_connection, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Could not set authorization token.",
                            _curl_buffer_error};
    }

    debuglog << "Set authorization token.\n";
}

void CURLWrapper::set_cainfo(const string_view path)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    CURLcode code{curl_easy_setopt(_connection, CURLOPT_CAINFO, path.data())};
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Could not set CA info.", _curl_buffer_error};
    }
}

void CURLWrapper::set_useragent(const string_view useragent)
{
    CURLcode code{
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(_connection, CURLOPT_USERAGENT, useragent.data())};
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set User-Agent",
                            _curl_buffer_error};
    }
    debuglog << "Set User-Agent to: " << useragent << '\n';
}

size_t CURLWrapper::writer_body(char *data, size_t size, size_t nmemb)
{
    if (data == nullptr)
    {
        return 0;
    }

    _buffer_mutex.lock();
    _curl_buffer_body.append(data, size * nmemb);
    _buffer_mutex.unlock();

    return size * nmemb;
}

size_t CURLWrapper::writer_header(char *data, size_t size, size_t nmemb)
{
    if (data == nullptr)
    {
        return 0;
    }

    _curl_buffer_headers.append(data, size * nmemb);

    return size * nmemb;
}

int CURLWrapper::progress(void *, curl_off_t, curl_off_t, curl_off_t,
                          curl_off_t) const
{
    if (_stream_cancelled)
    {
        return 1;
    }
    return 0;
}

void CURLWrapper::setup_curl()
{
    if (_connection == nullptr)
    {
        throw CURLException{CURLE_FAILED_INIT, "Failed to initialize curl."};
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_ERRORBUFFER, _curl_buffer_error);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_WRITEFUNCTION, writer_body_wrapper);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_WRITEDATA, this);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_HEADERFUNCTION,
                     writer_header_wrapper);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_HEADERDATA, this);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_XFERINFOFUNCTION, progress_wrapper);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_XFERINFODATA, this);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_NOPROGRESS, 0L);

    CURLWrapper::set_useragent((string("mastodonpp/") += version));

    // The next 2 only fail if HTTP is not supported.
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    CURLcode code{curl_easy_setopt(_connection, CURLOPT_FOLLOWLOCATION, 1L)};
    if (code != CURLE_OK)
    {
        throw CURLException{code, "HTTP is not supported.", _curl_buffer_error};
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    curl_easy_setopt(_connection, CURLOPT_MAXREDIRS, 10L);
}

bool CURLWrapper::replace_parameter_in_uri(string &uri,
                                           const parameterpair &parameter)
{
    static constexpr array replace{"id",
                                   "nickname",
                                   "nickname_or_id",
                                   "account_id",
                                   "list_id",
                                   "hashtag",
                                   "permission_group",
                                   "instance",
                                   "report_id",
                                   "name",
                                   "emoji"};
    if (any_of(replace.begin(), replace.end(),
               [&parameter](const auto &s) { return s == parameter.first; }))
    {
        const string searchstring{[&parameter] {
            string s{"<"};
            s += parameter.first;
            transform(s.begin(), s.end(), s.begin(),
                      [](const unsigned char c) { return toupper(c); });
            return s;
        }()};
        const auto pos{uri.find(searchstring)};
        if (pos != string::npos)
        {
            uri.replace(pos, parameter.first.size() + 2,
                        get<string_view>(parameter.second));
            debuglog << "Replaced :" << parameter.first << " in URI with "
                     << get<string_view>(parameter.second) << '\n';
            return true;
        }
    }

    return false;
}

void CURLWrapper::add_parameters_to_uri(string &uri,
                                        const parametermap &parameters)
{
    bool first{true};
    // Replace <ID> with the value of parameter “id” and so on.
    for (const auto &param : parameters)
    {
        if (replace_parameter_in_uri(uri, param))
        {
            continue;
        }

        if (first)
        {
            uri += "?";
            first = false;
        }
        else
        {
            uri += "&";
        }
        if (holds_alternative<string_view>(param.second))
        {
            ((uri += param.first) += "=") += get<string_view>(param.second);
        }
        else
        {
            for (const auto &arg : get<vector<string_view>>(param.second))
            {
                ((uri += param.first) += "[]=") += arg;
                if (arg != *get<vector<string_view>>(param.second).rbegin())
                {
                    uri += "&";
                }
            }
        }
    }
}

void CURLWrapper::add_mime_part(curl_mime *mime, string_view name,
                                string_view data)
{
    curl_mimepart *part{curl_mime_addpart(mime)};
    if (part == nullptr)
    {
        throw CURLException{"Could not build HTTP form."};
    }

    CURLcode code{curl_mime_name(part, name.data())};
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Could not build HTTP form."};
    }

    if (data.substr(0, 6) == "@file:")
    {
        const string_view filename{data.substr(6)};
        code = curl_mime_filedata(part, filename.data());
    }
    else
    {
        code = curl_mime_data(part, data.data(), CURL_ZERO_TERMINATED);
    }
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Could not build HTTP form."};
    }

    debuglog << "Set form part: " << name << " = " << data << '\n';
}

curl_mime *CURLWrapper::parameters_to_curl_mime(string &uri,
                                                const parametermap &parameters)
{
    debuglog << "Building HTTP form.\n";

    curl_mime *mime{curl_mime_init(_connection)};

    for (const auto &param : parameters)
    {
        if (replace_parameter_in_uri(uri, param))
        {
            continue;
        }

        if (holds_alternative<string_view>(param.second))
        {
            add_mime_part(mime, param.first, get<string_view>(param.second));
        }
        else
        {
            for (const auto &arg : get<vector<string_view>>(param.second))
            {
                const string_view name{string(param.first) += "[]"};
                add_mime_part(mime, name, arg);
            }
        }
    }

    return mime;
}

} // namespace mastodonpp
