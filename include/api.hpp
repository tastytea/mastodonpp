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

namespace mastodonpp::API
{

/*!
 *  @brief  A list of all v1 API calls.
 *
 *  The original `/` are substituted by `_`.
 *
 *  @since  0.1.0
 */
enum class v1
{
    instance
};

} // namespace mastodonpp::API

#endif  // MASTODONPP_API_HPP
