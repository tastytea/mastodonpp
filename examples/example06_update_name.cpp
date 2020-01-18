/*  This file is part of mastodonpp.
 *  Copyright © 2020 tastytea <tastytea@tastytea.de>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 *  SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 *  OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 *  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

// Update account display name settings (/api/v1/accounts/update_credentials).

#if __has_include("mastodonpp.hpp")
#    include "mastodonpp.hpp"            // We're building mastodonpp.
#else
#    include <mastodonpp/mastodonpp.hpp> // We're building outside mastodonpp.
#endif

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace masto = mastodonpp;
using std::cout;
using std::cerr;
using std::endl;
using std::to_string;
using std::string_view;
using std::vector;

int main(int argc, char *argv[])
{
    const vector<string_view> args(argv, argv + argc);
    if (args.size() <= 3)
    {
        cerr << "Usage: " << args[0]
             << " <instance hostname> <access token> <name>\n";
        return 1;
    }
    const auto name{args[3]};

    try
    {
        // Initialize an Instance and a Connection.
        masto::Instance instance{args[1], args[2]};
        masto::Connection connection{instance};

        // Update the settings.
        const auto answer{connection.patch(
                masto::API::v1::accounts_update_credentials,
                {
                    {"display_name", name}
                })};
        if (answer)
        {
            cout << "Successfully changed display name.\n";
        }
        else
        {
            if (answer.curl_error_code == 0)
            {
                // If it is no libcurl error, it must be an HTTP error.
                cerr << "HTTP status: " << answer.http_status << endl;
            }
            else
            {
                // Network errors like “Couldn't resolve host.”.
                cerr << "libcurl error " << to_string(answer.curl_error_code)
                     << ": " << answer.error_message << endl;
            }
        }
    }
    catch (const masto::CURLException &e)
    {
        // Only libcurl errors that are not network errors will be thrown.
        // There went probably something wrong with the initialization.
        cerr << e.what() << endl;
    }

    return 0;
}
