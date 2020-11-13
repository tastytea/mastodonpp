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

#ifndef MASTODONPP_INSTANCE_HPP
#define MASTODONPP_INSTANCE_HPP

#include "curl_wrapper.hpp"
#include "types.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace mastodonpp
{

using std::string;
using std::string_view;
using std::uint64_t;
using std::vector;

/*!
 *  @brief  Holds the access data of an instance.
 *
 *  Instance%s are needed to initialize Connection%s. All properties you set
 *  here (with set_proxy(), set_useragent() and so on) are copied to every
 *  Connection you initialize afterwards.
 *
 *  @since  0.1.0
 *
 *  @headerfile instance.hpp mastodonpp/instance.hpp
 */
class Instance : public CURLWrapper
{
public:
    /*!
     *  @brief  Construct a new Instance object.
     *
     *  @param  hostname     The hostname of the instance.
     *  @param  access_token Your access token.
     *
     *  @since  0.1.0
     */
    explicit Instance(string_view hostname, string_view access_token);

    /*!
     *  @brief  Copy constructor. A new CURLWrapper is constructed.
     *
     *  @since  0.5.2
     */
    Instance(const Instance &other);

    //! Move constructor
    Instance(Instance &&other) noexcept = delete;

    //! Destructor
    ~Instance() noexcept override = default;

    //! Copy assignment operator
    Instance &operator=(const Instance &other) = delete;

    //! Move assignment operator
    Instance &operator=(Instance &&other) noexcept = delete;

    /*!
     *  @brief  Set the properties of the connection of the calling class up.
     *
     *  Meant for internal use. This aligns the properties of the connection of
     *  the calling class with the properties of connection of this class.
     *
     *  @param  curlwrapper The CURLWrapper parent of the calling class.
     *
     *  @since  0.3.0
     */
    inline void copy_connection_properties(CURLWrapper &curlwrapper) const
    {
        curlwrapper.setup_connection_properties(_proxy, _access_token, _cainfo,
                                                _useragent);
    }

    /*!
     *  @brief  Returns the hostname.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] inline string_view get_hostname() const noexcept
    {
        return _hostname;
    }

    /*!
     *  @brief  Returns the base URI.
     *
     *  The base URI is “https://” + the hostname.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] inline string_view get_baseuri() const noexcept
    {
        return _baseuri;
    }

    /*!
     *  @brief  Returns the access token.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] inline string_view get_access_token() const noexcept
    {
        return _access_token;
    }

    /*!
     *  @brief  Set OAuth 2.0 Bearer Access Token.
     *
     *  Sets also the access token for all Connection%s that are initialized
     *  with this Instance afterwards.
     *
     *  @since  0.1.0
     */
    inline void set_access_token(const string_view access_token)
    {
        _access_token = access_token;
        CURLWrapper::set_access_token(access_token);
    }

    /*!
     *  @brief  Returns the maximum number of characters per post.
     *
     *  Queries `/api/v1/instance` for `max_toot_chars'. If the instance doesn't
     *  support it, the limit is assumed to be 500.
     *
     *  After the first call, the value is saved internally. Subsequent calls
     *  return the saved value.
     *
     *  @since  0.1.0
     */
    [[nodiscard]] uint64_t get_max_chars() noexcept;

    /*! @copydoc CURLWrapper::set_proxy(string_view)
     *
     *  Sets also the proxy for all Connection%s that are initialized with this
     *  Instance afterwards.
     */
    void set_proxy(const string_view proxy) override
    {
        _proxy = proxy;
        CURLWrapper::set_proxy(proxy);
    }

    /*!
     *  @brief  Returns the NodeInfo of the instance.
     *
     *  Attempts to download the [NodeInfo]
     *  (https://nodeinfo.diaspora.software/protocol.html) of the instance and
     *  returns it. Not every instance has it.
     *
     *  @since  0.3.0
     */
    [[nodiscard]] answer_type get_nodeinfo();

    /*!
     *  @brief  Returns the allowed mime types for statuses.
     *
     *  Extracts `metadata.postFormats` from NodeInfo. If none can be found,
     *  returns `{"text/plain"}`.
     *
     *  After the first call, the value is saved internally. Subsequent calls
     *  return the saved value.
     *
     *  @since  0.3.0
     */
    vector<string> get_post_formats() noexcept;

    /*!
     *  @brief  Set path to Certificate Authority (CA) bundle.
     *
     *  Sets also the CA info for all Connection%s that are initialized with
     *  this Instance afterwards.
     *
     *  @since  0.3.0
     */
    void set_cainfo(string_view path) override
    {
        _cainfo = path;
        CURLWrapper::set_cainfo(path);
    }

    /*!
     *  @brief  Sets the User-Agent.
     *
     *  Sets also the User-Agent for all Connection%s that are initialized with
     *  this Instance afterwards.
     *
     *  @since  0.3.0
     */
    void set_useragent(const string_view useragent) override
    {
        _useragent = useragent;
        CURLWrapper::set_useragent(useragent);
    }

    /*!
     *  @brief  Simplifies obtaining an OAuth 2.0 Bearer Access Token.
     *
     *  * Create an Instance and initialize this class with it.
     *  * Call step_1() to get the URI your user has to visit.
     *  * Get the authorization code from your user.
     *  * Call step_2() with the code.
     *
     *  Example:
     *  @code
     *  mastodonpp::Instance instance{"example.com", {}};
     *  mastodonpp::Instance::ObtainToken token{instance};
     *  auto answer{token.step1("Good program", "read:blocks read:mutes", "")};
     *  if (answer)
     *  {
     *      std::cout << "Please visit " << answer << "\nand paste the code: ";
     *      std::string code;
     *      std::cin >> code;
     *      answer = access_token{token.step2(code)};
     *      if (answer)
     *      {
     *          std::cout << "Success!\n";
     *      }
     *  }
     *  @endcode
     *
     *  @since  0.3.0
     *
     *  @headerfile instance.hpp mastodonpp/instance.hpp
     */
    class ObtainToken : public CURLWrapper
    {
    public:
        /*!
         *  @brief  Constructor.
         *
         *  @since  0.3.0
         */
        explicit ObtainToken(Instance &instance)
            : _instance{instance}
            , _baseuri{instance.get_baseuri()}
        {
            _instance.copy_connection_properties(*this);
        }

        /*!
         *  @brief  Creates an application via `/api/v1/apps`.
         *
         *  The `body` of the returned @link answer_type answer @endlink
         *  contains only the URI, not the whole JSON response.
         *
         *  Note that the required scopes may be different between Mastodon and
         *  other implementations, like Pleroma.
         *
         *  @param  client_name The name of your application.
         *  @param  scopes      Space separated list of scopes. Defaults to
         *                      “read” if empty.
         *  @param  website     The URI to the homepage of your application. Can
         *                      be an empty string.
         *
         *  @return The URI your user has to visit.
         *
         *  @since  0.3.0
         */
        [[nodiscard]] answer_type step_1(string_view client_name,
                                         string_view scopes,
                                         string_view website);

        /*!
         *  @brief  Creates a token via `/oauth/token`.
         *
         *  The `body` of the returned @link answer_type answer @endlink
         *  contains only the access token, not the whole JSON response.
         *
         *  The access token will be set in the Instance you initialized
         *  this ObtainToken with.
         *
         *  @param  code The authorization code you got from the user.
         *
         *  @return The access token.
         *
         *  @since  0.3.0
         */
        [[nodiscard]] answer_type step_2(string_view code);

    private:
        Instance &_instance;
        const string _baseuri;
        string _scopes;
        string _client_id;
        string _client_secret;
    };

private:
    const string _hostname;
    const string _baseuri;
    string _access_token;
    uint64_t _max_chars;
    string _proxy;
    vector<string> _post_formats;
    string _cainfo;
    string _useragent;
};

} // namespace mastodonpp

#endif // MASTODONPP_INSTANCE_HPP
