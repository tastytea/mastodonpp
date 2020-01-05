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

#ifndef MASTODONPP_CONNECTION_HPP
#define MASTODONPP_CONNECTION_HPP

#include "api.hpp"
#include "curl_wrapper.hpp"
#include "instance.hpp"
#include "return_types.hpp"

#include <string>
#include <string_view>

namespace mastodonpp
{

using std::string;
using std::string_view;

/*!
 *  @brief  Represents a connection to an instance. Used for requests.
 *
 *  @since  0.1.0
 *
 *  @headerfile connection.hpp mastodonpp/connection.hpp
 */
class Connection : public CURLWrapper
{
public:
    /*!
     *  @brief  Construct a new Connection object.
     *
     *  @param  instance An Instance with the access data.
     *
     *  @since  0.1.0
     */
    explicit Connection(Instance &instance);

    /*!
     *  @brief  Make a HTTP GET call.
     *
     *  @param endpoint Endpoint as API::endpoint_type, for example:
     *                  `mastodonpp::API::v1::instance`.
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    answer_type get(const API::endpoint_type &endpoint);

    /*!
     *  @brief  Make a HTTP GET call.
     *
     *  @param  endpoint Endpoint as string, for example: "/api/v1/instance".
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    answer_type get(const string_view &endpoint);

private:
    Instance &_instance;
    const string_view _baseuri;
    const API _api;
};

} // namespace mastodonpp

#endif  // MASTODONPP_CONNECTION_HPP
