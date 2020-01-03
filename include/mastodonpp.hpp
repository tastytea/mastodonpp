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

#ifndef MASTODONPP_HPP
#define MASTODONPP_HPP

#include "return_types.hpp"
#include <string>

/*!
 *  @mainpage mastodonpp Reference
 *
 *  @section using Using the library
 *
 *  Include mastodonpp.hpp, which then includes all other headers.
 *
 *  @code
 *  #include <mastodonpp/mastodonpp.hpp>
 *  @endcode
 *
 *  Use it in your CMake project like this:
 *
 *  @code
 *  find_package(mastodonpp REQUIRED CONFIG)
 *  target_link_libraries(MyProject mastodonpp::mastodonpp)
 *  @endcode
 *
 *  Or compile your code with `g++ $(pkg-config --cflags --libs mastodonpp)`.
 *
 *  @section Example
 *
 *  @code
 *  mastodonpp::API masto("example.com", "");
 *  @endcode
 */

namespace mastodonpp
{

using std::string;

/*!
 *  @brief  C++ wrapper for the Mastodon API.
 *
 *  All text input is expected to be UTF-8.
 *
 *  @since  0.1.0
 *
 */

class API
{
public:
    /*!
     *  @brief  Construct a new API object.
     *
     *  To register your application, leave access_token blank and call
     *  API::register_app1() and API::register_app2().
     *
     *  @param  instance     The hostname of your instance.
     *  @param  access_token Your access token.
     *
     *  @since  0.1.0
     */
    explicit API(string instance, string access_token);

private:
    const string _instance;
    const string _access_token;
};

} // namespace mastodonpp

#endif  // MASTODONPP_HPP
