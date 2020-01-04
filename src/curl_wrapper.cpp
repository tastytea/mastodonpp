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

namespace mastodonpp
{

CURLWrapper::CURLWrapper()
    : _curl_buffer_error{}
{
    curl_global_init(CURL_GLOBAL_ALL); // NOLINT(hicpp-signed-bitwise)
    _connection = curl_easy_init();
    setup_curl();
}
CURLWrapper::~CURLWrapper() noexcept
{
    curl_easy_cleanup(_connection);
    curl_global_cleanup();
}

string CURLWrapper::make_request(const http_method &meth,
                                 const string_view &uri)
{
    CURLcode code;
    switch (meth)
    {
    case http_method::GET:
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        code = curl_easy_setopt(_connection, CURLOPT_HTTPGET, 1L);
        break;
    }
    case http_method::POST:
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        code = curl_easy_setopt(_connection, CURLOPT_POST, 1L);
        break;
    }
    }
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set HTTP method",
                _curl_buffer_error};
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    code = curl_easy_setopt(_connection, CURLOPT_URL, uri.data());
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set URI", _curl_buffer_error};
    }

    code = curl_easy_perform(_connection);
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to perform request",
                _curl_buffer_error};
    }

    return _curl_buffer;
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
    code = curl_easy_setopt(_connection, CURLOPT_WRITEDATA, &_curl_buffer);
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set write data",
                _curl_buffer_error};
    }
}

} // namespace mastodonpp
