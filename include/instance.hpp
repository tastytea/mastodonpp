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

#ifndef MASTODONPP_INSTANCE_HPP
#define MASTODONPP_INSTANCE_HPP

#include "curl_wrapper.hpp"
#include "answer.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>

namespace mastodonpp
{

using std::uint64_t;
using std::string;
using std::string_view;
using std::move;

/*!
 *  @brief  Holds the access data of an instance.
 *
 *  @since  0.1.0
 *
 *  @headerfile instance.hpp mastodonpp/instance.hpp
 */
class Instance : public CURLWrapper
{
public:
    /*!
     *  @brief  Construct a new Instance object.
     *
     *  @param  hostname     The hostname of the instance.
     *  @param  access_token Your access token.
     *
     *  @since  0.1.0
     */
    explicit Instance(string_view hostname, string_view access_token);

    /*!
     *  @brief  Returns the hostname.
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    inline string_view get_hostname() const
    {
        return _hostname;
    }

    /*!
     *  @brief  Returns the base URI.
     *
     *  The base URI is “https://” + the hostname.
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    inline string_view get_baseuri() const
    {
        return _baseuri;
    }

    /*!
     *  @brief  Returns the access token.
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    inline string_view get_access_token() const
    {
        return _access_token;
    }

    /*!
     *  @brief  Set OAuth 2.0 Bearer Access Token.
     *
     *  Sets also the access token for all Connection%s that are initialized
     *  with this Instance afterwards.
     *
     *  @since  0.1.0
     */
    inline void set_access_token(string access_token)
    {
        _access_token = move(access_token);
    }

    /*!
     *  @brief  Returns the maximum number of characters per post.
     *
     *  Queries `/api/v1/instance` for `max_toot_chars'. If the instance doesn't
     *  support it, the limit is assumed to be 500.
     *
     *  After the first call, the value is saved internally. Subsequent calls
     *  return the saved value.
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    uint64_t get_max_chars();

    /*! @copydoc CURLWrapper::set_proxy(string_view)
     *
     *  Sets also the proxy for all Connection%s that are initialized with this
     *  Instance afterwards.
     */
    void set_proxy(const string_view proxy)
    {
        _proxy = proxy;
        CURLWrapper::set_proxy(proxy);
    }

    /*!
     *  @brief  Returns the proxy string that was previously set.
     *
     *  Does not return the proxy if it was set from an environment variable.
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    string_view get_proxy() const
    {
        return _proxy;
    }

    /*!
     *  @brief  Returns the NodeInfo of the instance.
     *
     *  Attempts to download the [NodeInfo]
     *  (https://nodeinfo.diaspora.software/protocol.html) of the instance and
     *  returns it. Not every instance has it.
     *
     *  @since  0.3.0
     */
    [[nodiscard]]
    answer_type get_nodeinfo();

private:
    const string _hostname;
    const string _baseuri;
    string _access_token;
    uint64_t _max_chars;
    string _proxy;
};

} // namespace mastodonpp

#endif  // MASTODONPP_INSTANCE_HPP
