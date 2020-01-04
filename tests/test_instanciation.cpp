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

#include "instance.hpp"
#include "request.hpp"

#include <catch.hpp>

#include <exception>
#include <string>

namespace mastodonpp
{

using std::string;

SCENARIO ("Instantiations.")
{
    bool exception = false;

    WHEN ("Instance is instantiated.")
    {
        try
        {
            Instance instance{"example.com", ""};
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN ("No exception is thrown")
        {
            REQUIRE_FALSE(exception);
        }
    }

    WHEN ("Request is instantiated.")
    {
        try
        {
            Instance instance{"example.com", ""};
            Request request{instance};
        }
        catch (const std::exception &e)
        {
            exception = true;
        }

        THEN ("No exception is thrown")
        {
            REQUIRE_FALSE(exception);
        }
    }
}

} // namespace mastodonpp
