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

#ifndef MASTODONPP_REQUEST_HPP
#define MASTODONPP_REQUEST_HPP

#include "api.hpp"
#include "curl_wrapper.hpp"
#include "instance.hpp"
#include "return_types.hpp"

#include <string>

namespace mastodonpp
{

using std::string;

/*!
 *  @brief  Used to make a request to the Mastodon %API.
 *
 *  @since  0.1.0
 *
 *  @headerfile request.hpp mastodonpp/request.hpp
 */
class Request : public CURLWrapper
{
public:
    /*!
     *  @brief  Construct a new Request object.
     *
     *  @param  instance An Instance with the access data.
     *
     *  @since  0.1.0
     */
    explicit Request(Instance &instance);

    /*!
     *  @brief  Make a HTTP GET call.
     *
     *  @param endpoint Endpoint as API::endpoint_type, for example:
     *                  `mastodonpp::API::v1::instance`.
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    answer_type get(API::endpoint_type endpoint) const;

    /*!
     *  @brief  Make a HTTP GET call.
     *
     *  @param  endpoint Endpoint as string, for example: "/api/v1/instance".
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    answer_type get(string endpoint) const;

private:
    Instance &_instance;
};

} // namespace mastodonpp

#endif  // MASTODONPP_REQUEST_HPP
