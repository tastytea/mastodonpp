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

// Post a status (/api/v1/status) with an attachment (/api/v1/media).

#if __has_include("mastodonpp.hpp")
#    include "mastodonpp.hpp" // We're building mastodonpp.
#else
#    include <mastodonpp/mastodonpp.hpp> // We're building outside mastodonpp.
#endif

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace masto = mastodonpp;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::string_view;
using std::to_string;
using std::vector;

int main(int argc, char *argv[])
{
    const vector<string_view> args(argv, argv + argc);
    if (args.size() <= 3)
    {
        cerr << "Usage: " << args[0]
             << " <instance hostname> <access token> <file>\n";
        return 1;
    }

    try
    {
        // Initialize an Instance and a Connection.
        masto::Instance instance{args[1], args[2]};
        masto::Connection connection{instance};
        const string_view filename{args[3]};

        // Create attachment.
        auto answer{connection.post(masto::API::v1::media,
                                    {{"file", string("@file:") += filename},
                                     {"description", "Test."}})};

        // Get the ID of the attachment.
        // You normally would use a JSON parser, of course. I don't use one
        // because I don't want to add a dependency just for an example.
        const auto pos{answer.body.find(R"("id":")") + 6};
        const auto endpos{answer.body.find(R"(",)", pos)};
        const auto media_id{answer.body.substr(pos, endpos - pos)};
        cout << "Attachment has ID: " << media_id << endl;

        // Post the status. Note that “media_ids” always has to be a vector.
        answer = connection.post(masto::API::v1::statuses,
                                 {{"status", "Attachment test."},
                                  {"media_ids",
                                   vector<string_view>{media_id}}});
        if (answer)
        {
            cout << "Successfully posted " << filename << ".\n";
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
