/*  This file is part of mastodonpp.
 *  Copyright © 2020, 2022 tastytea <tastytea@tastytea.de>
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

#include "instance.hpp"

// catch 3 does not have catch.hpp anymore
#if __has_include(<catch.hpp>)
#    include <catch.hpp>
#else
#    include <catch_all.hpp>
#endif

#include <exception>
#include <string>

namespace mastodonpp
{

using std::string;

SCENARIO("mastopp::Instance")
{
    bool exception = false;

    WHEN("Instance is instantiated.")
    {
        try
        {
            Instance instance{"example.com", {}};
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN("No exception is thrown")
        {
            REQUIRE_FALSE(exception);
        }
    }

    WHEN("Variables are set.")
    {
        constexpr auto hostname{"likeable.space"};
        constexpr auto proxy{"socks4a://[::1]:9050"};
        constexpr auto access_token{"abc123"};
        Instance instance{hostname, {}};

        try
        {
            instance.set_proxy(proxy);
            instance.set_access_token(access_token);
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN("No exception is thrown")
        AND_THEN("get_access_token() returns the set value.")
        AND_THEN("get_hostname() returns the set value.")
        AND_THEN("get_baseuri() returns the expected value.")
        {
            REQUIRE_FALSE(exception);
            REQUIRE(instance.get_access_token() == access_token);
            REQUIRE(instance.get_hostname() == hostname);
            REQUIRE(instance.get_baseuri() == (string("https://") += hostname));
        }
    }
}

} // namespace mastodonpp
