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

#include "helpers.hpp"

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

SCENARIO("mastodonpp::html_unescape()")
{
    bool exception = false;

    WHEN("html_unescape() is called.")
    {
        string result;
        try
        {
            result = unescape_html("2&euro; = 2&#8364; = 2&#x20ac;");
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN("No exception is thrown")
        AND_THEN("Result is as expected.")
        {
            REQUIRE_FALSE(exception);
            REQUIRE(result == "2€ = 2€ = 2€");
        }
    }
}

} // namespace mastodonpp
