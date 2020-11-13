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

// Obtain an access token and verify that it works.

#if __has_include("mastodonpp.hpp")
#    include "mastodonpp.hpp" // We're building mastodonpp.
#else
#    include <mastodonpp/mastodonpp.hpp> // We're building outside mastodonpp.
#endif

#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace masto = mastodonpp;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::exit;
using std::string;
using std::string_view;
using std::to_string;
using std::vector;

void handle_error(const masto::answer_type &answer);

int main(int argc, char *argv[])
{
    const vector<string_view> args(argv, argv + argc);
    if (args.size() <= 1)
    {
        cerr << "Usage: " << args[0] << " <instance hostname>\n";
        return 1;
    }

    try
    {
        // Initialize Instance and Instance::ObtainToken.
        masto::Instance instance{args[1], {}};
        masto::Instance::ObtainToken token{instance};

        // Create an “Application” (/api/v1/apps),
        // and get URI for the authorization code (/oauth/authorize).
        // NOTE: Mastodon only needs read:accounts for verify_credentials but
        // Pleroma needs the full read scope.
        auto answer{token.step_1("Testclient", "read write:favourites",
                                 "https://example.com/")};
        if (!answer)
        {
            handle_error(answer);
        }

        cout << "Please visit " << answer << "\nand paste the code here: ";
        string code;
        cin >> code;

        // Obtain the token (/oauth/token).
        answer = token.step_2(code);
        if (!answer)
        {
            handle_error(answer);
        }

        cout << "Your access token is: " << answer << endl;

        // Test if the token works.
        masto::Connection connection{instance};
        answer = connection.get(masto::API::v1::apps_verify_credentials);
        if (!answer)
        {
            handle_error(answer);
        }

        cout << answer << endl;
    }
    catch (const masto::CURLException &e)
    {
        // Only libcurl errors that are not network errors will be thrown.
        // There went probably something wrong with the initialization.
        cerr << e.what() << endl;
    }

    return 0;
}

void handle_error(const masto::answer_type &answer)
{
    if (answer.curl_error_code == 0)
    {
        // If it is no libcurl error, it must be an HTTP error.
        cerr << "HTTP status: " << answer.http_status << endl;
    }
    else
    {
        // Network errors like “Couldn't resolve host.”.
        cerr << "libcurl error " << to_string(answer.curl_error_code) << ": "
             << answer.error_message << endl;
    }

    exit(1);
}
