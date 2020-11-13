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

// Get the last 4 public statuses of an instance and process them with
// nlohmann-json. <https://github.com/nlohmann/json>

#if __has_include("mastodonpp.hpp")
#    include "mastodonpp.hpp" // We're building mastodonpp.
#else
#    include <mastodonpp/mastodonpp.hpp> // We're building outside mastodonpp.
#endif

// Don't compile if nlohmann-json can't be found.
#if __has_include(<nlohmann/json.hpp>)
#    include <nlohmann/json.hpp>

#    include <cstdlib>
#    include <iostream>
#    include <string>
#    include <string_view>
#    include <vector>

namespace masto = mastodonpp;
using json = nlohmann::json;
using std::cerr;
using std::cout;
using std::exit;
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
        // Initialize Instance and Connection.
        masto::Instance instance{args[1], {}};
        masto::Connection connection{instance};

        // Get the last 4 public statuses of the instance.
        auto answer{connection.get(masto::API::v1::timelines_public,
                                   {{"limit", "4"}, {"local", "true"}})};
        if (answer)
        {
            // Parse JSON string.
            auto statuses{json::parse(answer.body)};
            for (const auto &status : statuses)
            {
                // Extract the info we want and print it.
                const auto acct{status["account"]["acct"].get<string_view>()};
                const auto content{status["content"].get<string_view>()};
                const auto id{status["id"].get<string_view>()};
                cout << acct << " wrote status " << id << ": \n";
                cout << "  " << content.substr(0, 76) << " …\n";

                // Print tags if there are any.
                const auto tags{status["tags"]};
                if (!tags.empty())
                {
                    cout << "    Tags: ";
                    for (const auto &tag : tags)
                    {
                        cout << '#' << tag["name"].get<string_view>() << " ";
                    }
                    cout << '\n';
                }

                // Print the number of attachments.
                const auto n_attachments{status["media_attachments"].size()};
                if (n_attachments > 0)
                {
                    cout << "    " << n_attachments << " attachment";
                    if (n_attachments > 1)
                    {
                        cout << "s";
                    }
                    cout << ".\n";
                }
            }
        }
        else
        {
            handle_error(answer);
        }
    }
    catch (const masto::CURLException &e)
    {
        // Only libcurl errors that are not network errors will be thrown.
        // There went probably something wrong with the initialization.
        cerr << e.what() << '\n';
    }
    catch (const nlohmann::detail::exception &e)
    {
        cerr << "JSON exception: " << e.what() << '\n';
    }
}

void handle_error(const masto::answer_type &answer)
{
    if (answer.curl_error_code == 0)
    {
        // If it is no libcurl error, it must be an HTTP error.
        cerr << "HTTP status: " << answer.http_status << '\n';
    }
    else
    {
        // Network errors like “Couldn't resolve host.”.
        cerr << "libcurl error " << to_string(answer.curl_error_code) << ": "
             << answer.error_message << '\n';
    }

    exit(1);
}

#else

#    include <iostream>
int main()
{
    std::cout << "Example could not be compiled "
                 "because nlohmann-json was not found.\n";
}

#endif // __has_include(<nlohmann/json.hpp>)
