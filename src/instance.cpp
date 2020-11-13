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

#include "log.hpp"

#include <algorithm>
#include <exception>
#include <regex>

namespace mastodonpp
{

using std::exception;
using std::regex;
using std::regex_search;
using std::smatch;
using std::sort;
using std::stoull;

Instance::Instance(const string_view hostname, const string_view access_token)
    : _hostname{hostname}
    , _baseuri{"https://" + _hostname}
    , _max_chars{0}
{
    set_access_token(access_token);
}

Instance::Instance(const Instance &other)
    : CURLWrapper{other}
    , _hostname{other._hostname}
    , _baseuri{other._baseuri}
    , _access_token{other._access_token}
    , _max_chars{other._max_chars}
    , _proxy{other._proxy}
    , _post_formats{other._post_formats}
    , _cainfo{other._cainfo}
    , _useragent{other._useragent}
{
    CURLWrapper::setup_connection_properties(_proxy, _access_token, _cainfo,
                                             _useragent);
}

uint64_t Instance::get_max_chars() noexcept
{
    constexpr uint64_t default_max_chars{500};

    if (_max_chars != 0)
    {
        return _max_chars;
    }

    try
    {
        debuglog << "Querying " << _hostname << " for max_toot_chars…\n";
        const auto answer{
            make_request(http_method::GET, _baseuri + "/api/v1/instance", {})};
        if (!answer)
        {
            debuglog << "Could not get instance info.\n";
            return default_max_chars;
        }

        // clang-format off
        _max_chars = [&answer]
        {
            // clang-format on
            const regex re_chars{R"("max_toot_chars"\s*:\s*([^"]+))"};
            smatch match;

            if (regex_search(answer.body, match, re_chars))
            {
                return static_cast<uint64_t>(stoull(match[1].str()));
            }

            debuglog << "max_toot_chars not found.\n";
            return default_max_chars;
        }();
        debuglog << "Set _max_chars to: " << _max_chars << '\n';
    }
    catch (const exception &e)
    {
        debuglog << "Unexpected exception: " << e.what() << '\n';
        return default_max_chars;
    }

    return _max_chars;
}

answer_type Instance::get_nodeinfo()
{
    auto answer{
        make_request(http_method::GET, _baseuri + "/.well-known/nodeinfo", {})};
    if (!answer)
    {
        debuglog << "NodeInfo not found.\n";
        return answer;
    }

    vector<string> hrefs;
    const regex re_href{R"("href"\s*:\s*"([^"]+)\")"};
    smatch match;
    string body = answer.body;
    while (regex_search(body, match, re_href))
    {
        hrefs.push_back(match[1].str());
        debuglog << "Found href: " << hrefs.back() << '\n';
        body = match.suffix();
    }
    sort(hrefs.begin(), hrefs.end()); // We assume they are sortable strings.
    debuglog << "Selecting href: " << hrefs.back() << '\n';

    return make_request(http_method::GET, hrefs.back(), {});
}

vector<string> Instance::get_post_formats() noexcept
{
    constexpr auto default_value{"text/plain"};

    if (!_post_formats.empty())
    {
        return _post_formats;
    }

    try
    {
        debuglog << "Querying " << _hostname << " for postFormats…\n";
        const auto answer{get_nodeinfo()};
        if (!answer)
        {
            debuglog << "Couldn't get NodeInfo.\n";
            _post_formats = {default_value};
            return _post_formats;
        }

        const regex re_allformats(R"("postFormats"\s*:\s*\[([^\]]+)\])");
        smatch match;
        if (!regex_search(answer.body, match, re_allformats))
        {
            debuglog << "Couldn't find metadata.postFormats.\n";
            _post_formats = {default_value};
            return _post_formats;
        }
        string allformats{match[1].str()};
        debuglog << "Found postFormats: " << allformats << '\n';

        const regex re_format(R"(\s*"([^"]+)\"\s*,?)");

        while (regex_search(allformats, match, re_format))
        {
            _post_formats.push_back(match[1].str());
            allformats = match.suffix();
            debuglog << "Found postFormat: " << _post_formats.back() << '\n';
        }
    }
    catch (const std::exception &e)
    {
        debuglog << "Unexpected exception: " << e.what() << '\n';
        return {default_value};
    }

    return _post_formats;
}

answer_type Instance::ObtainToken::step_1(const string_view client_name,
                                          const string_view scopes,
                                          const string_view website)
{
    parametermap parameters{{"client_name", client_name},
                            {"redirect_uris", "urn:ietf:wg:oauth:2.0:oob"}};
    if (!scopes.empty())
    {
        _scopes = scopes;
        parameters.insert({"scopes", scopes});
    }
    if (!website.empty())
    {
        parameters.insert({"website", website});
    }

    auto answer{
        make_request(http_method::POST, _baseuri + "/api/v1/apps", parameters)};
    if (answer)
    {
        const regex re_id{R"("client_id"\s*:\s*"([^"]+)\")"};
        const regex re_secret{R"("client_secret"\s*:\s*"([^"]+)\")"};
        smatch match;

        if (regex_search(answer.body, match, re_id))
        {
            _client_id = match[1].str();
        }
        if (regex_search(answer.body, match, re_secret))
        {
            _client_secret = match[1].str();
        }

        string uri{_baseuri + "/oauth/authorize?scope=" + escape_url(scopes)
                   + "&response_type=code"
                     "&redirect_uri="
                   + escape_url("urn:ietf:wg:oauth:2.0:oob")
                   + "&client_id=" + _client_id};
        if (!website.empty())
        {
            uri += "&website=" + escape_url(website);
        }
        answer.body = uri;
        debuglog << "Built URI.";
    }

    return answer;
}

answer_type Instance::ObtainToken::step_2(const string_view code)
{
    parametermap parameters{{"client_id", _client_id},
                            {"client_secret", _client_secret},
                            {"redirect_uri", "urn:ietf:wg:oauth:2.0:oob"},
                            {"code", code},
                            {"grant_type", "authorization_code"}};
    if (!_scopes.empty())
    {
        parameters.insert({"scope", _scopes});
    }

    auto answer{
        make_request(http_method::POST, _baseuri + "/oauth/token", parameters)};
    if (answer)
    {
        const regex re_token{R"("access_token"\s*:\s*"([^"]+)\")"};
        smatch match;

        if (regex_search(answer.body, match, re_token))
        {
            answer.body = match[1].str();
            debuglog << "Got access token.\n";
            _instance.set_access_token(answer.body);
        }
    }

    return answer;
}

} // namespace mastodonpp
