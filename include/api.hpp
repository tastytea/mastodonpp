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

#ifndef MASTODONPP_API_HPP
#define MASTODONPP_API_HPP

#include <map>
#include <string>
#include <variant>

namespace mastodonpp
{

using std::string;
using std::variant;

/*!
 *  @brief  Holds API endpoints.
 *
 *  @since  0.1.0
 *
 *  @headerfile api.hpp mastodonpp/api.hpp
 */
class API
{
public:
    /*!
     *  @brief  An enumeration of all v1 %API endpoints.
     *
     *  The original `/` are substituted with `_`.
     *
     *  @since  0.1.0
     */
    enum class v1
    {
        instance
    };

    /*!
     *  @brief  An enumeration of all v2 %API endpoints.
     *
     *  The original `/` are substituted with `_`.
     *
     *  @since  0.1.0
     */
    enum class v2
    {
        search
    };

    /*!
     *  @brief  Type for endpoints. Either API::v1 or API::v2.
     *
     *  @since  0.1.0
     */
    using endpoint_type = variant<v1,v2>;

    /*!
     *  @brief  Constructs an API object. You should never need this.
     *
     *  This constructor exists to hide away the class members, which are used
     *  internally.
     *
     *  @since  0.1.0
     */
    explicit API(endpoint_type &endpoint);

    /*!
     *  @brief  Convert #endpoint_type to string.
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    string to_string() const;

private:
    const endpoint_type _endpoint;
};

} // namespace mastodonpp

#endif  // MASTODONPP_API_HPP
