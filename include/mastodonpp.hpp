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
#include "helpers.hpp"
#include "instance.hpp"
#include "types.hpp"

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
 *  Since we use C++17 features in the headers of this library, your program
 *  needs to be compiled as C++17 or higher too.
 *
 *  @subsection example Example
 *
 *  @code
 *  #include <mastodonpp/mastodonpp.hpp>
 *  #include <iostream>
 *
 *  int main()
 *  {
 *      mastodonpp::Instance instance{"example.com", {}};
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
 *  @subsection input Input
 *
 *  * All text input is expected to be UTF-8.
 *  * To send a file, use “<tt>\@file:</tt>” followed by the file name as value
 *    in the @link mastodonpp::parametermap parametermap@endlink.
 *
 *  @section exceptions Exceptions
 *
 *  Any unrecoverable libcurl error will be thrown as a
 *  mastodonpp::CURLException. Network errors will not be thrown, but reported
 *  via the return value.
 *
 *  @section thread_safety Thread safety
 *
 *  The first time you construct an @link mastodonpp::Instance Instance@endlink,
 *  [curl_global_init(3)](https://curl.haxx.se/libcurl/c/curl_global_init.html)
 *  is called. When the last @link mastodonpp::Instance Instance @endlink is
 *  destroyed, [curl_global_cleanup(3)]
 *  (https://curl.haxx.se/libcurl/c/curl_global_cleanup.html) is called. Both
 *  are not thread safe.
 *
 *  Do not make 2 requests with the same @link mastodonpp::Connection Connection
 *  @endlink at the same time. You can create as many @link
 *  mastodonpp::Connection Connection@endlink%s as you want from one @link
 *  mastodonpp::Instance Instance@endlink.
 *
 *  If you are using libcurl with OpenSSL before 1.1.0, please read
 *  [libcurl-thread(3)](https://curl.haxx.se/libcurl/c/threadsafe.html).
 *
 *  @example example01_instance_info.cpp
 *  @example example02_streaming.cpp
 *  @example example03_post_status.cpp
 *  @example example04_post_with_attachment.cpp
 *  @example example05_update_notification_settings.cpp
 *  @example example06_update_name.cpp
 *  @example example07_delete_status.cpp
 *  @example example08_obtain_token.cpp
 *  @example example09_nlohmann_json.cpp
 */

/*!
 *  @brief  C++ wrapper for the Mastodon %API.
 *
 *  @since  0.1.0
 *
 *  @headerfile mastodonpp.hpp mastodonpp/mastodonpp.hpp
 */
namespace mastodonpp
{} // namespace mastodonpp

#endif // MASTODONPP_HPP
