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

#include <string>
#include <string_view>

namespace mastodonpp
{

using std::string;
using std::string_view;

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
    explicit Instance(string hostname, string access_token);

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

private:
    const string _hostname;
    const string _baseuri;
    string _access_token;
};

} // namespace mastodonpp

#endif  // MASTODONPP_INSTANCE_HPP
