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

#include "instance.hpp"
#include "exceptions.hpp"

#include <utility>

namespace mastodonpp
{

using std::move;

Instance::Instance(string instance, string access_token)
    : _instance{move(instance)}
    , _access_token{move(access_token)}
    , _connection{curl_easy_init()}
{
    setup_curl();
}
Instance::~Instance()
{
    curl_easy_cleanup(_connection);
}

int Instance::writer(char *data, size_t size, size_t nmemb, string *writerData)
{
    if(writerData == nullptr)
    {
        return 0;
    }

    writerData->append(data, size*nmemb);

    return static_cast<int>(size * nmemb);
}

void Instance::setup_curl()
{
    if (_connection == nullptr)
    {
        throw CURLException{CURLE_FAILED_INIT, "Failed to initialize curl."};
    }

    CURLcode code{curl_easy_setopt(_connection, CURLOPT_ERRORBUFFER,
                                   _curl_buffer_error)};
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set error buffer."};
    }

    code = curl_easy_setopt(_connection, CURLOPT_WRITEFUNCTION, writer);
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set writer", _curl_buffer_error};
    }

    code = curl_easy_setopt(_connection, CURLOPT_WRITEDATA, &_curl_buffer);
    if (code != CURLE_OK)
    {
        throw CURLException{code, "Failed to set write data",
                _curl_buffer_error};
    }
}

} // namespace mastodonpp
