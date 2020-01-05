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

#ifndef MASTODONPP_API_HPP
#define MASTODONPP_API_HPP

#include <map>
#include <string_view>
#include <variant>

namespace mastodonpp
{

using std::string_view;
using std::variant;

/*!
 *  @brief  Holds %API endpoints.
 *
 *  Supported %API endpoints: Mastodon 3.0.
 *
 *  @since  0.1.0
 *
 *  @headerfile api.hpp mastodonpp/api.hpp
 */
class API
{
public:
    /*!
     *  @brief  An enumeration of all v1 %API endpoints.
     *
     *  The original `/` are substituted with `_`.
     *
     *  @since  0.1.0
     */
    enum class v1
    {
        apps,
        apps_verify_credentials,

        accounts,
        accounts_verify_credentials,
        accounts_update_credentials,
        accounts_id,
        accounts_id_statuses,
        accounts_id_followers,
        accounts_id_following,
        accounts_id_lists,
        accounts_id_identity_proofs,
        accounts_id_follow,
        accounts_id_unfollow,
        accounts_id_block,
        accounts_id_unblock,
        accounts_id_mute,
        accounts_id_unmute,
        accounts_id_pin,
        accounts_id_unpin,
        accounts_relationships,
        accounts_search,

        bookmarks,

        favourites,

        mutes,

        blocks,

        domain_blocks,

        filters,
        filters_id,

        reports,

        follow_requests,
        follow_requests_id_authorize,
        follow_requests_id_reject,

        endorsements,

        featured_tags,
        featured_tags_id,
        featured_tags_suggestions,

        preferences,

        suggestions,
        suggestions_account_id,

        statuses,
        statuses_id,
        statuses_id_context,
        statuses_id_reblogged_by,
        statuses_id_favourited_by,
        statuses_id_favourite,
        statuses_id_unfavourite,
        statuses_id_reblog,
        statuses_id_unreblog,
        statuses_id_bookmark,
        statuses_id_unbookmark,
        statuses_id_mute,
        statuses_id_unmute,
        statuses_id_pin,
        statuses_id_unpin,

        media,
        media_id,

        polls_id,
        polls_id_votes,

        scheduled_statuses,
        scheduled_statuses_id,

        timelines_public,
        timelines_tag_hashtag,
        timelines_home,
        timelines_list_list_id,

        conversations,
        conversations_id,
        conversations_id_read,

        lists,
        lists_id,
        lists_id_accounts,

        markers,

        streaming_health,
        streaming_user,
        streaming_public,
        streaming_public_local,
        streaming_hashtag,
        streaming_hashtag_local,
        streaming_list,
        streaming_direct,

        notifications,
        notifications_id,
        notifications_clear,
        notifications_id_dismiss,

        push_subscription,

        instance,
        instance_peers,
        instance_activity,

        trends,

        directory,

        custom_emojis,

        admin_accounts,
        admin_accounts_id,
        admin_accounts_account_id_action,
        admin_accounts_id_approve,
        admin_accounts_id_reject,
        admin_accounts_id_enable,
        admin_accounts_id_unsilence,
        admin_accounts_id_unsuspend,
        admin_reports,
        admin_reports_id,
        admin_reports_id_assign_to_self,
        admin_reports_id_unassign,
        admin_reports_id_resolve,
        admin_reports_id_reopen
    };

    /*!
     *  @brief  An enumeration of all v2 %API endpoints.
     *
     *  The original `/` are substituted with `_`.
     *
     *  @since  0.1.0
     */
    enum class v2
    {
        search
    };

    /*!
     *  @brief  An enumeration of all oauth %API endpoints.
     *
     *  The original `/` are substituted with `_`.
     *
     *  @since  0.1.0
     */
    enum class oauth
    {
        authorize,
        token,
        revoke
    };

    /*!
     *  @brief  An enumeration of all other %API endpoints.
     *
     *  These endpoints are directly under `/api/`.
     *
     *  The original `/` are substituted with `_`.
     *
     *  @since  0.1.0
     */
    enum class other
    {
        proofs,
        oembed
    };

    /*!
     *  @brief  Type for endpoints. Can be API::v1, API::v2, API::oauth or
     *          API::other.
     *
     *  @since  0.1.0
     */
    using endpoint_type = variant<v1,v2,oauth,other>;

    /*!
     *  @brief  Constructs an API object. You should never need this.
     *
     *  This constructor exists to hide away the class members, which are used
     *  internally.
     *
     *  @since  0.1.0
     */
    explicit API(const endpoint_type &endpoint);

    /*!
     *  @brief  Convert #endpoint_type to `std::string_view`.
     *
     *  @since  0.1.0
     */
    [[nodiscard]]
    string_view to_string_view() const;

private:
    const endpoint_type _endpoint;
};

} // namespace mastodonpp

#endif  // MASTODONPP_API_HPP
