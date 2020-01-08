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

#include "answer.hpp"
#include "api.hpp"
#include "curl_wrapper.hpp"
#include "instance.hpp"

#include <string>
#include <string_view>
#include <variant>

namespace mastodonpp
{

using std::string;
using std::string_view;
using std::variant;

/*!
 *  @brief  An endpoint. Either API::endpoint_type or `std::string_view`.
 *
 *  @since  0.1.0
 */
using endpoint_variant = variant<API::endpoint_type,string_view>;

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
     *  @brief  Make a HTTP GET call with parameters.
     *
     *  Example:
     *  @code
     *  auto answer{connection.get(mastodonpp::API::v1::accounts_id_followers,
     *                             {
     *                                 {"id", "12"},
     *                                 {"limit", "10"}
     *                             })};
     *  @endcode
     *
     *  @param endpoint   Endpoint as API::endpoint_type or `std::string_view`.
     *  @param parameters A map of parameters.
     *
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    answer_type get(const endpoint_variant &endpoint,
                    const parametermap &parameters);

    /*!
     *  @brief  Make a HTTP GET call.
     *  Example:
     *  @code
     *  auto answer{connection.get("/api/v1/instance")};
     *  @endcode
     *
     *  @param endpoint Endpoint as API::endpoint_type or `std::string_view`.
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    inline answer_type get(const endpoint_variant &endpoint)
    {
        return get(endpoint, {});
    }

    /*! @copydoc CURLWrapper::set_proxy(string_view)
     *
     *  Sets also the proxy for the Instance you used to initialize this
     *  Connection.
     */
    void set_proxy(const string_view proxy);

private:
    Instance &_instance;
    const string_view _baseuri;
};

} // namespace mastodonpp

#endif  // MASTODONPP_CONNECTION_HPP
