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
#include <vector>

namespace mastodonpp
{

using std::string;
using std::string_view;
using std::variant;
using std::vector;

/*!
 *  @brief  An endpoint. Either API::endpoint_type or `std::string_view`.
 *
 *  @since  0.1.0
 */
using endpoint_variant = variant<API::endpoint_type,string_view>;

/*!
 *  @brief  A stream event.
 *
 *  @since  0.1.0
 *
 *  @headerfile connection.hpp mastodonpp/connection.hpp
 */
struct event_type
{
    /*!
     *  @brief  The type of the event.
     *
     *  Can be: “update”, “notification”, “delete” or “filters_changed”. For
     *  more information consult [the Mastodon documentation]
     *  (https://docs.joinmastodon.org/methods/timelines/streaming/
     *  #event-types-a-idevent-typesa).
     */
    string type;

    //! The payload.
    string data;
};

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
     *
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

    /*!
     *  @brief  Make a HTTP POST call with parameters.
     *
     *  Example:
     *  @code
     *  auto answer{connection.post(
     *          mastodonpp::API::v1::statuses,
     *          {
     *              {"status", "How is the wheather?"},
     *              {"poll[options]", vector<string_view>{"Nice", "not nice"}},
     *              {"poll[expires_in]", to_string(poll_seconds)}
     *          })};
     *  @endcode
     *
     *  @param endpoint   Endpoint as API::endpoint_type or `std::string_view`.
     *  @param parameters A map of parameters.
     *
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    answer_type post(const endpoint_variant &endpoint,
                     const parametermap &parameters);

    /*!
     *  @brief  Make a HTTP POST call.
     *
     *  @param endpoint Endpoint as API::endpoint_type or `std::string_view`.
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    inline answer_type post(const endpoint_variant &endpoint)
    {
        return post(endpoint, {});
    }

    /*!
     *  @brief  Copy new stream contents and delete the “original”.
     *
     *  Note that the last event is not necessarily complete, it could happen
     *  that you are calling this function mid-transfer. You have to check the
     *  data integrity yourself.
     *
     *  Using get_new_events() instead is recommended.
     *
     *  @since  0.1.0
     */
    string get_new_stream_contents();

    /*!
     *  @brief  Get new stream events.
     *
     *  @since  0.1.0
     */
    vector<event_type> get_new_events();

    //! @copydoc CURLWrapper::cancel_stream
    inline void cancel_stream()
    {
        CURLWrapper::cancel_stream();
    }
private:
    Instance &_instance;
    const string_view _baseuri;

    [[nodiscard]]
    string endpoint_to_uri(const endpoint_variant &endpoint) const;
};

} // namespace mastodonpp

#endif  // MASTODONPP_CONNECTION_HPP
