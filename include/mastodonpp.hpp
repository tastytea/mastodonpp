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

#include "api.hpp"
#include "connection.hpp"
#include "exceptions.hpp"
#include "instance.hpp"
#include "return_types.hpp"

/*!
 *  @headerfile mastodonpp.hpp mastodonpp/mastodonpp.hpp
 *
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
 *  @subsection Example
 *
 *  @code
 *  #include <mastodonpp/mastodonpp.hpp>
 *  #include <iostream>
 *
 *  int main()
 *  {
 *      mastodonpp::Instance instance{"example.com", ""};
 *      std::cout << "Maximum characters per post: "
 *                << instance.get_max_chars() << std::endl;
 *
 *      mastodonpp::Connection connection{instance};
 *
 *      auto answer{connection.get(mastodonpp::API::v1::instance)};
 *      if (answer)
 *      {
 *          std::cout << answer << std::endl;
 *      }
 *  }
 *  @endcode
 *
 *  @section exceptions Exceptions
 *
 *  Any unrecoverable libcurl error will be thrown as a
 *  mastodonpp::CURLException. Network errors will **not** be thrown, but
 *  reported via the return value.
 */

/*!
 *  @brief  C++ wrapper for the Mastodon %API.
 *
 *  All text input is expected to be UTF-8.
 *
 *  @since  0.1.0
 */
namespace mastodonpp
{



} // namespace mastodonpp

#endif  // MASTODONPP_HPP
