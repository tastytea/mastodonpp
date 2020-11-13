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

#ifndef MASTODONPP_HELPERS_HPP
#define MASTODONPP_HELPERS_HPP

#include <string>
#include <string_view>

//! @headerfile helpers.hpp mastodonpp/helpers.hpp

namespace mastodonpp
{

using std::string;

/*!
 *  @brief  Replaces HTML entities with UTF-8 characters.
 *
 *  Supports named and numbered entities, decimal and hexadecimal.
 *
 *  Example:
 *  @code
 *  // Will output: 2€ = 2€ = 2€
 *  std::cout << mastodonpp::unescape_html("2&euro; = 2&#8364; = 2&#x20ac;");
 *  @endcode
 *
 *  @param  html The HTML to unescape.
 *
 *  @since  0.4.0
 */
[[nodiscard]] string unescape_html(string html);

} // namespace mastodonpp

#endif // MASTODONPP_HELPERS_HPP
