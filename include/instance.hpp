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

#include <curl/curl.h>

#include <string>

namespace mastodonpp
{

using std::string;

/*!
 *  @brief  Holds the access data of and the connection to an instance.
 *
 *  @since  0.1.0
 *
 *  @headerfile instance.hpp mastodonpp/instance.hpp
 */
class Instance
{
public:
    /*!
     *  @brief  Construct a new Instance object.
     *
     *  @param  instance     The hostname of the instance.
     *  @param  access_token Your access token.
     *
     *  @since  0.1.0
     */
    explicit Instance(string instance, string access_token);

    //! Copy constructor
    Instance(const Instance &other) = default;

    //! Move constructor
    Instance(Instance &&other) = default;

    //! Destructor
    virtual ~Instance();

    //! Copy assignment operator
    Instance& operator=(const Instance &other) = delete;

    //! Move assignment operator
    Instance& operator=(Instance &&other) = delete;

private:
    const string _instance;
    string _access_token;
    CURL *_connection;
    char _curl_buffer_error[CURL_ERROR_SIZE];
    string _curl_buffer;


    /*!
     *  @brief  libcurl write callback function.
     *
     *  @since  0.1.0
     */
    static int writer(char *data, size_t size, size_t nmemb,
                      string *writerData);

    /*!
     *  @brief  Setup libcurl connection.
     *
     *  @since  0.1.0
     */
    void setup_curl();
};

} // namespace mastodonpp

#endif  // MASTODONPP_INSTANCE_HPP
