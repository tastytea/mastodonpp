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

#include <atomic>
#include <cstdint>
#include <cstring>

namespace mastodonpp
{

using std::get;
using std::holds_alternative;
using std::atomic;
using std::uint8_t;
using std::uint16_t;

static atomic<uint16_t> curlwrapper_instances{0};

CURLWrapper::CURLWrapper()
    : _curl_buffer_error{}
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

answer_type CURLWrapper::make_request(const http_method &method, string uri,
                                      const parametermap &parameters)
{
    CURLcode code;
    switch (method)
    {
    case http_method::GET:
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        code = curl_easy_setopt(_connection, CURLOPT_HTTPGET, 1L);

        for (const auto &param : parameters)
        {
            static bool first{true};
            if (first)
            {
                uri.append("?");
                first = false;
            }
            else
            {
                uri.append("&");
            }
            if (holds_alternative<string>(param.second))
            {
                uri.append(param.first + '=' + get<string>(param.second));
            }
            else
            {
                for (const auto &arg : get<vector<string>>(param.second))
                {
                    uri.append(param.first + "[]=" + arg);
                    if (arg != *get<vector<string>>(param.second).rbegin())
                    {
                        uri.append("&");
                    }
                }
            }
        }

        break;
    }
    case http_method::POST:
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        code = curl_easy_setopt(_connection, CURLOPT_POST, 1L);
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
    if (code == CURLE_OK)
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

int CURLWrapper::writer(char *data, size_t size, size_t nmemb,
                        string *writerData)
{
    if(writerData == nullptr)
    {
        return 0;
    }

    writerData->append(data, size*nmemb);

    return static_cast<int>(size * nmemb);
}

void CURLWrapper::setup_curl()
{
    if (_connection == nullptr)
    {
        throw CURLException{CURLE_FAILED_INIT, "Failed to initialize curl."};
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    CURLcode code{curl_easy_setopt(_connection, CURLOPT_ERRORBUFFER,
                                   _curl_buffer_error)};
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set error buffer."};
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    code = curl_easy_setopt(_connection, CURLOPT_WRITEFUNCTION, writer);
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set writer", _curl_buffer_error};
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    code = curl_easy_setopt(_connection, CURLOPT_HEADERDATA,
                            &_curl_buffer_headers);
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set header data",
                _curl_buffer_error};
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    code = curl_easy_setopt(_connection, CURLOPT_WRITEDATA, &_curl_buffer_body);
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set write data",
                _curl_buffer_error};
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    code = curl_easy_setopt(_connection, CURLOPT_USERAGENT,
                            string("mastorss/").append(version).c_str());
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

} // namespace mastodonpp
