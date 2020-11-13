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

using std::map;
using std::string_view;
using std::variant;

/*!
 *  @brief  Holds %API endpoints.
 *
 *  Supported %API endpoints: Mastodon 3.0.1, Pleroma 2.0.2.
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
     *  The original `/` are substituted with `_`. `:` are omitted.
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
        admin_reports_id_reopen,

        pleroma_notifications_read,
        pleroma_accounts_id_subscribe,
        pleroma_accounts_id_unsubscribe,
        pleroma_accounts_id_favourites,
        pleroma_accounts_update_avatar,
        pleroma_accounts_update_banner,
        pleroma_accounts_update_background,
        pleroma_accounts_confirmation_resend,
        pleroma_mascot,
        pleroma_conversations_id_statuses,
        pleroma_conversations_id,
        pleroma_conversations_id_read,
        pleroma_accounts_id_scrobbles,
        pleroma_scrobble,
        pleroma_statuses_id_reactions_emoji,
        pleroma_statuses_id_reactions,
    };

    /*!
     *  @brief  An enumeration of all v2 %API endpoints.
     *
     *  The original `/` are substituted with `_`. `:` are omitted.
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
     *  The original `/` are substituted with `_`. `:` are omitted.
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
     *  The original `/` are substituted with `_`. `:` are omitted.
     *
     *  @since  0.1.0
     */
    enum class other
    {
        proofs,
        oembed
    };

    /*!
     *  @brief  An enumeration of all pleroma %API endpoints.
     *
     *  The original `/` are substituted with `_`. `:` are omitted.
     *
     *  @since  0.1.0
     */
    enum class pleroma
    {
        admin_users,
        admin_users_follow,
        admin_users_unfollow,
        admin_users_nickname_toggle_activation,
        admin_users_tag,
        admin_users_nickname_permission_group,
        admin_users_nickname_permission_group_permission_group,
        admin_users_permission_group_permission_group,
        admin_users_activate,
        admin_users_deactivate,
        admin_users_nickname_or_id,
        admin_users_nickname_or_id_statuses,
        admin_instances_instance_statuses,
        admin_statuses,
        admin_relay,
        admin_users_invite_token,
        admin_users_invites,
        admin_users_revoke_invite,
        admin_users_email_invite,
        admin_users_nickname_password_reset,
        admin_users_nickname_update_credentials,
        admin_users_force_password_reset,
        admin_reports,
        admin_grouped_reports,
        admin_reports_id,
        admin_reports_id_notes,
        admin_reports_report_id_notes_id,
        admin_statuses_id,
        admin_restart,
        admin_config,
        admin_config_descriptions,
        admin_moderation_log,
        admin_reload_emoji,
        admin_users_confirm_email,
        admin_users_resend_confirm_email,
        admin_stats,

        // No longer documented for Pleroma 2.0.0.
        admin_users_nickname,
        admin_users_nickname_activation_status,
        admin_reports_id_respond,
        admin_config_migrate_to_db,
        admin_config_migrate_from_db,

        emoji,
        follow_import,
        captcha,
        delete_account,
        disable_account,
        notification_settings,
        healthcheck,
        change_email,
        emoji_packs,
        emoji_packs_name,
        emoji_packs_name_update_file,
        emoji_packs_name_update_metadata,
        emoji_packs_download_from,
        emoji_packs_list_from,
        emoji_packs_name_download_shared,

        // No longer documented for Pleroma 2.0.0.
        account_register,

    };

    /*!
     *  @brief  Type for endpoints. Can be API::v1, API::v2, API::oauth,
     *          API::other or API::pleroma.
     *
     *  @since  0.1.0
     */
    using endpoint_type = variant<v1, v2, oauth, other, pleroma>;

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
    [[nodiscard]] inline string_view to_string_view() const
    {
        return _endpoint_map.at(_endpoint);
    }

private:
    const endpoint_type _endpoint;
    static const map<endpoint_type, string_view> _endpoint_map;
};

} // namespace mastodonpp

#endif // MASTODONPP_API_HPP
