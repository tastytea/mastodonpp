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

// Print new public events (/api/v1/streaming/public).

#if __has_include("mastodonpp.hpp")
#    include "mastodonpp.hpp" // We're building mastodonpp.
#else
#    include <mastodonpp/mastodonpp.hpp> // We're building outside mastodonpp.
#endif

#include <chrono>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

namespace masto = mastodonpp;
using namespace std::chrono_literals;
using std::cerr;
using std::cout;
using std::endl;
using std::string_view;
using std::thread;
using std::to_string;
using std::vector;
using std::this_thread::sleep_for;

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
        // Initialize an Instance.
        masto::Instance instance{args[1], {}};

        // Initialize a Connection.
        masto::Connection connection{instance};

        // Find out if the streaming service is fine.
        auto answer{connection.get(masto::API::v1::streaming_health)};
        if (answer && answer.body == "OK")
        {
            // Make a thread, get all public events.
            // clang-format off
            thread stream_thread{[&]
            {
                // clang-format on
                answer = connection.get(masto::API::v1::streaming_public);
            }};

            // Print new events every 2 seconds, for 10 seconds.
            for (auto counter{0}; counter < 5; ++counter)
            {
                sleep_for(2s);
                for (const auto &event : connection.get_new_events())
                {
                    // Print type of event and the beginning of the data.
                    cout << event.type << ": " << event.data.substr(0, 70)
                         << " …" << endl;
                }
            }

            // Cancel the stream, …
            connection.cancel_stream();
            // … and wait for the thread.
            stream_thread.join();
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
