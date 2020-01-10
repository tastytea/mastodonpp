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
#include <cstdint>
#include <cstring>

namespace mastodonpp
{

using std::get;
using std::holds_alternative;
using std::any_of;
using std::array;
using std::atomic;
using std::uint8_t;
using std::uint16_t;

// No one will ever need more than 65535 connections. 😉
static atomic<uint16_t> curlwrapper_instances{0};

CURLWrapper::CURLWrapper()
    : _curl_buffer_error{}
    , _stream_cancelled(false)
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

void CURLWrapper::set_proxy(const string_view proxy)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    CURLcode code{curl_easy_setopt(_connection, CURLOPT_PROXY, proxy)};
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set proxy", _curl_buffer_error};
    }
}

void CURLWrapper::cancel_stream()
{
    _stream_cancelled = true;
}

answer_type CURLWrapper::make_request(const http_method &method, string uri,
                                      const parametermap &parameters)
{
    _stream_cancelled = false;
    _curl_buffer_headers.clear();
    _curl_buffer_body.clear();

    CURLcode code;
    switch (method)
    {
    case http_method::GET:
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        code = curl_easy_setopt(_connection, CURLOPT_HTTPGET, 1L);

        add_parameters_to_uri(uri, parameters);

        break;
    }
    case http_method::POST:
    {
        curl_mime *mime{parameters_to_curl_mime(uri, parameters)};

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        code = curl_easy_setopt(_connection, CURLOPT_MIMEPOST, mime);

        break;
    }
    case http_method::PATCH:
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        code = curl_easy_setopt(_connection, CURLOPT_CUSTOMREQUEST, "PATCH");
        break;
    }
    case http_method::PUT:
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        code = curl_easy_setopt(_connection, CURLOPT_UPLOAD, 1L);
        break;
    }
    case http_method::DELETE:
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        code = curl_easy_setopt(_connection, CURLOPT_CUSTOMREQUEST, "DELETE");
        break;
    }
    }
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set HTTP method",
                _curl_buffer_error};
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
        long http_status;       // NOLINT(google-runtime-int)
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

size_t CURLWrapper::writer_body(char *data, size_t size, size_t nmemb)
{
    if(data == nullptr)
    {
        return 0;
    }

    buffer_mutex.lock();
    _curl_buffer_body.append(data, size * nmemb);
    buffer_mutex.unlock();

    return size * nmemb;
}

size_t CURLWrapper::writer_header(char *data, size_t size, size_t nmemb)
{
    if(data == nullptr)
    {
        return 0;
    }

    _curl_buffer_headers.append(data, size * nmemb);

    return size * nmemb;
}

int CURLWrapper::progress(void *, curl_off_t , curl_off_t ,
                          curl_off_t , curl_off_t )
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

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    CURLcode code{curl_easy_setopt(_connection, CURLOPT_USERAGENT,
                                   (string("mastodonpp/") += version).c_str())};
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set User-Agent",
                _curl_buffer_error};
    }

    // The next 2 only fail if HTTP is not supported.
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    code = curl_easy_setopt(_connection, CURLOPT_FOLLOWLOCATION, 1L);
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
    static constexpr array replace
        {
            "id", "nickname", "nickname_or_id",
            "hashtag", "permission_group"
        };
    if (any_of(replace.begin(), replace.end(),
               [&parameter](const auto &s) { return s == parameter.first; }))
    {
        const auto pos{uri.find('<')};
        if (pos != string::npos)
        {
            uri.replace(pos, parameter.first.size() + 2,
                        get<string_view>(parameter.second));
            return true;
        }
    }

    return false;
}

void CURLWrapper::add_parameters_to_uri(string &uri,
                                        const parametermap &parameters)
{
    // Replace <ID> with the value of parameter “id” and so on.
    for (const auto &param : parameters)
    {
        if (replace_parameter_in_uri(uri, param))
        {
            continue;
        }

        static bool first{true};
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

        curl_mimepart *part{curl_mime_addpart(mime)};
        if (part == nullptr)
        {
            throw CURLException{"Could not build HTTP form."};
        }

        CURLcode code;
        if (holds_alternative<string_view>(param.second))
        {
            code = curl_mime_name(part, param.first.data());
            if (code != CURLE_OK)
            {
                throw CURLException{code, "Could not build HTTP form."};
            }

            code = curl_mime_data(part, get<string_view>(param.second).data(),
                                  CURL_ZERO_TERMINATED);
            if (code != CURLE_OK)
            {
                throw CURLException{code, "Could not build HTTP form."};
            }
            debuglog << "Set form part: " << param.first << " = "
                     << get<string_view>(param.second) << '\n';
        }
        else
        {
            for (const auto &arg : get<vector<string_view>>(param.second))
            {
                const string name{string(param.first) += "[]"};
                code = curl_mime_name(part, name.c_str());
                if (code != CURLE_OK)
                {
                    throw CURLException{code, "Could not build HTTP form."};
                }
                code = curl_mime_data(part, arg.data(), CURL_ZERO_TERMINATED);
                if (code != CURLE_OK)
                {
                    throw CURLException{code, "Could not build HTTP form."};
                }
                debuglog << "Set form part: " << name << " = " << arg << '\n';
            }
        }
    }

    return mime;
}

} // namespace mastodonpp
