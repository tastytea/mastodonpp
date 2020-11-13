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

#include "api.hpp"

namespace mastodonpp
{

API::API(const endpoint_type &endpoint)
    : _endpoint{endpoint}
{}

// TODO: look for a better way.
// NOLINTNEXTLINE(cert-err58-cpp)
const map<API::endpoint_type, string_view> API::_endpoint_map{
    {v1::apps, "/api/v1/apps"},
    {v1::apps_verify_credentials, "/api/v1/apps/verify_credentials"},

    {v1::accounts, "/api/v1/accounts"},
    {v1::accounts_verify_credentials, "/api/v1/accounts/verify_credentials"},
    {v1::accounts_update_credentials, "/api/v1/accounts/update_credentials"},
    {v1::accounts_id, "/api/v1/accounts/<ID>"},
    {v1::accounts_id_statuses, "/api/v1/accounts/<ID>/statuses"},
    {v1::accounts_id_followers, "/api/v1/accounts/<ID>/followers"},
    {v1::accounts_id_following, "/api/v1/accounts/<ID>/following"},
    {v1::accounts_id_lists, "/api/v1/accounts/<ID>/lists"},
    {v1::accounts_id_identity_proofs, "/api/v1/accounts/<ID>/identity_proofs"},
    {v1::accounts_id_follow, "/api/v1/accounts/<ID>/follow"},
    {v1::accounts_id_unfollow, "/api/v1/accounts/<ID>/unfollow"},
    {v1::accounts_id_block, "/api/v1/accounts/<ID>/block"},
    {v1::accounts_id_unblock, "/api/v1/accounts/<ID>/unblock"},
    {v1::accounts_id_mute, "/api/v1/accounts/<ID>/mute"},
    {v1::accounts_id_unmute, "/api/v1/accounts/<ID>/unmute"},
    {v1::accounts_id_pin, "/api/v1/accounts/<ID>/pin"},
    {v1::accounts_id_unpin, "/api/v1/accounts/<ID>/unpin"},
    {v1::accounts_relationships, "/api/v1/accounts/relationships"},
    {v1::accounts_search, "/api/v1/accounts/search"},

    {v1::bookmarks, "/api/v1/bookmarks"},

    {v1::favourites, "/api/v1/favourites"},

    {v1::mutes, "/api/v1/mutes"},

    {v1::blocks, "/api/v1/blocks"},

    {v1::domain_blocks, "/api/v1/domain_blocks"},

    {v1::filters, "/api/v1/filters"},
    {v1::filters_id, "/api/v1/filters/<ID>"},

    {v1::reports, "/api/v1/reports"},

    {v1::follow_requests, "/api/v1/follow_requests"},
    {v1::follow_requests_id_authorize,
     "/api/v1/follow_requests/<ID>/authorize"},
    {v1::follow_requests_id_reject, "/api/v1/follow_requests/<ID>/reject"},

    {v1::endorsements, "/api/v1/endorsements"},

    {v1::featured_tags, "/api/v1/featured/tags"},
    {v1::featured_tags_id, "/api/v1/featured_tags/<ID>"},
    {v1::featured_tags_suggestions, "/api/v1/featured_tags/suggestions"},

    {v1::preferences, "/api/v1/preferences"},

    {v1::suggestions, "/api/v1/suggestions"},
    {v1::suggestions_account_id, "/api/v1/suggestions/account/<ID>"},

    {v1::statuses, "/api/v1/statuses"},
    {v1::statuses_id, "/api/v1/statuses/<ID>"},
    {v1::statuses_id_context, "/api/v1/statuses/<ID>/context"},
    {v1::statuses_id_reblogged_by, "/api/v1/statuses/<ID>/reblogged_by"},
    {v1::statuses_id_favourited_by, "/api/v1/statuses/<ID>/favourited_by"},
    {v1::statuses_id_favourite, "/api/v1/statuses/<ID>/favourite"},
    {v1::statuses_id_unfavourite, "/api/v1/statuses/<ID>/unfavourite"},
    {v1::statuses_id_reblog, "/api/v1/statuses/<ID>/reblog"},
    {v1::statuses_id_unreblog, "/api/v1/statuses/<ID>/unreblog"},
    {v1::statuses_id_bookmark, "/api/v1/statuses/<ID>/bookmark"},
    {v1::statuses_id_unbookmark, "/api/v1/statuses/<ID>/unbookmark"},
    {v1::statuses_id_mute, "/api/v1/statuses/<ID>/mute"},
    {v1::statuses_id_unmute, "/api/v1/statuses/<ID>/unmute"},
    {v1::statuses_id_pin, "/api/v1/statuses/<ID>/pin"},
    {v1::statuses_id_unpin, "/api/v1/statuses/<ID>/unpin"},

    {v1::media, "/api/v1/media"},
    {v1::media_id, "/api/v1/media/<ID>"},

    {v1::polls_id, "/api/v1/polls/<ID>"},
    {v1::polls_id_votes, "/api/v1/polls/<ID>/votes"},

    {v1::scheduled_statuses, "/api/v1/scheduled_statuses"},
    {v1::scheduled_statuses_id, "/api/v1/scheduled_statuses/<ID>"},

    {v1::timelines_public, "/api/v1/timelines/public"},
    {v1::timelines_tag_hashtag, "/api/v1/timelines/tag/<HASHTAG>"},
    {v1::timelines_home, "/api/v1/timelines/home"},
    {v1::timelines_list_list_id, "/api/v1/timelines/list/<LIST_ID>"},

    {v1::conversations, "/api/v1/conversations"},
    {v1::conversations_id, "/api/v1/conversations/<ID>"},
    {v1::conversations_id_read, "/api/v1/conversations/<ID>/read"},

    {v1::lists, "/api/v1/lists"},
    {v1::lists_id, "/api/v1/lists/id"},
    {v1::lists_id_accounts, "/api/v1/lists/<ID>/accounts"},

    {v1::markers, "/api/v1/markers"},

    {v1::streaming_health, "/api/v1/streaming/health"},
    {v1::streaming_user, "/api/v1/streaming/user"},
    {v1::streaming_public, "/api/v1/streaming/public"},
    {v1::streaming_public_local, "/api/v1/streaming/public/local"},
    {v1::streaming_hashtag, "/api/v1/streaming/hashtag"},
    {v1::streaming_hashtag_local, "/api/v1/streaming/hashtag/local"},
    {v1::streaming_list, "/api/v1/streaming/list"},
    {v1::streaming_direct, "/api/v1/streaming/direct"},

    {v1::notifications, "/api/v1/notifications"},
    {v1::notifications_id, "/api/v1/notifications/<ID>"},
    {v1::notifications_clear, "/api/v1/notifications/clear"},
    {v1::notifications_id_dismiss, "/api/v1/notifications/<ID>/dismiss"},

    {v1::push_subscription, "/api/v1/push/subscription"},

    {v1::instance, "/api/v1/instance"},
    {v1::instance_peers, "/api/v1/instance/peers"},
    {v1::instance_activity, "/api/v1/instance/activity"},

    {v1::trends, "/api/v1/trends"},

    {v1::directory, "/api/v1/directory"},

    {v1::custom_emojis, "/api/v1/custom_emojis"},

    {v1::admin_accounts, "/api/v1/admin/accounts"},
    {v1::admin_accounts_id, "/api/v1/admin/accounts/<ID>"},
    {v1::admin_accounts_account_id_action,
     "/api/v1/admin/accounts/<ACCOUNT_ID>/action"},
    {v1::admin_accounts_id_approve, "/api/v1/admin/accounts/<ID>/approve"},
    {v1::admin_accounts_id_reject, "/api/v1/admin/accounts/<ID>/reject"},
    {v1::admin_accounts_id_enable, "/api/v1/admin/accounts/<ID>/enable"},
    {v1::admin_accounts_id_unsilence, "/api/v1/admin/accounts/<ID>/unsilence"},
    {v1::admin_accounts_id_unsuspend, "/api/v1/admin/accounts/<ID>/unsuspend"},
    {v1::admin_reports, "/api/v1/admin/reports"},
    {v1::admin_reports_id, "/api/v1/admin/reports/<ID>"},
    {v1::admin_reports_id_assign_to_self,
     "/api/v1/admin/reports/<ID>/assign_to_self"},
    {v1::admin_reports_id_unassign, "/api/v1/admin/reports/<ID>/unassign"},
    {v1::admin_reports_id_resolve, "/api/v1/admin/reports/<ID>/resolve"},
    {v1::admin_reports_id_reopen, "/api/v1/admin/reports/<ID>/reopen"},

    {v1::pleroma_notifications_read, " /api/v1/pleroma/notifications/read"},
    {v1::pleroma_accounts_id_subscribe,
     "/api/v1/pleroma/accounts/<ID>/subscribe"},
    {v1::pleroma_accounts_id_unsubscribe,
     "/api/v1/pleroma/accounts/<ID>/unsubscribe"},
    {v1::pleroma_accounts_id_favourites,
     "/api/v1/pleroma/accounts/<ID>/favourites"},
    {v1::pleroma_accounts_update_avatar,
     "/api/v1/pleroma/accounts/update_avatar"},
    {v1::pleroma_accounts_update_banner,
     "/api/v1/pleroma/accounts/update_banner"},
    {v1::pleroma_accounts_update_background,
     "/api/v1/pleroma/accounts/update_background"},
    {v1::pleroma_accounts_confirmation_resend,
     "/api/v1/pleroma/accounts/confirmation_resend"},
    {v1::pleroma_mascot, "/api/v1/pleroma/mascot"},
    {v1::pleroma_conversations_id_statuses,
     "/api/v1/pleroma/conversations/<ID>/statuses"},
    {v1::pleroma_conversations_id, "/api/v1/pleroma/conversations/<ID>"},
    {v1::pleroma_conversations_id_read,
     "/api/v1/pleroma/conversations/<ID>/read"},
    {v1::pleroma_accounts_id_scrobbles,
     "/api/v1/pleroma/accounts/<ID>/scrobbles"},
    {v1::pleroma_scrobble, "/api/v1/pleroma/scrobble"},
    {v1::pleroma_statuses_id_reactions_emoji,
     "/api/v1/pleroma/statuses/<ID>/reactions/<EMOJI>"},
    {v1::pleroma_statuses_id_reactions,
     "/api/v1/pleroma/statuses/<ID>/reactions"},

    {v2::search, "/api/v2/search"},

    {oauth::authorize, "/oauth/authorize"},
    {oauth::token, "/oauth/token"},
    {oauth::revoke, "/oauth/revoke"},

    {other::proofs, "/api/proofs"},
    {other::oembed, "/api/oembed"},

    {pleroma::admin_users, "/api/pleroma/admin/users"},
    {pleroma::admin_users_follow, "/api/pleroma/admin/users/follow"},
    {pleroma::admin_users_unfollow, "/api/pleroma/admin/users/unfollow"},
    {pleroma::admin_users_nickname_toggle_activation,
     "/api/pleroma/admin/users/<NICKNAME>/toggle_activation"},
    {pleroma::admin_users_tag, "/api/pleroma/admin/users/tag"},
    {pleroma::admin_users_nickname_permission_group,
     "/api/pleroma/admin/users/<NICKNAME>/permission_group"},
    {pleroma::admin_users_nickname_permission_group_permission_group,
     "/api/pleroma/admin/users/<NICKNAME>"
     "/permission_group/<PERMISSION_GROUP>"},
    {pleroma::admin_users_permission_group_permission_group,
     "/api/pleroma/admin/users/permission_group/<PERMISSION_GROUP>"},
    {pleroma::admin_users_activate, "/api/pleroma/admin/users/activate"},
    {pleroma::admin_users_deactivate, "/api/pleroma/admin/users/deactivate"},
    {pleroma::admin_users_nickname_or_id,
     "/api/pleroma/admin/users/<NICKNAME_OR_ID>"},
    {pleroma::admin_users_nickname_or_id_statuses,
     "/api/pleroma/admin/users/<NICKNAME_OR_ID>/statuses"},
    {pleroma::admin_instances_instance_statuses,
     "/api/pleroma/admin/instances/<INSTANCE>/statuses"},
    {pleroma::admin_statuses, "/api/pleroma/admin/statuses"},
    {pleroma::admin_relay, "/api/pleroma/admin/relay"},
    {pleroma::admin_users_invite_token,
     "/api/pleroma/admin/users/invite_token"},
    {pleroma::admin_users_invites, "/api/pleroma/admin/users/invites"},
    {pleroma::admin_users_revoke_invite,
     "/api/pleroma/admin/users/revoke_invite"},
    {pleroma::admin_users_email_invite,
     "/api/pleroma/admin/users/email_invite"},
    {pleroma::admin_users_nickname_password_reset,
     "/api/pleroma/admin/users/<NICKNAME>/password_reset"},
    {pleroma::admin_users_nickname_update_credentials,
     "/api/pleroma/admin/users/<NICKNAME>/update_credentials"},
    {pleroma::admin_users_force_password_reset,
     "/api/pleroma/admin/users/force_password_reset"},
    {pleroma::admin_reports, "/api/pleroma/admin/reports"},
    {pleroma::admin_grouped_reports, "/api/pleroma/admin/grouped_reports"},
    {pleroma::admin_reports_id, "/api/pleroma/admin/reports/<ID>"},
    {pleroma::admin_reports_id_notes, "/api/pleroma/admin/reports/<ID>/notes"},
    {pleroma::admin_reports_report_id_notes_id,
     "/api/pleroma/admin/reports/<REPORT_ID>/notes/<ID>"},
    {pleroma::admin_statuses_id, "/api/pleroma/admin/statuses/<ID>"},
    {pleroma::admin_restart, "/api/pleroma/admin/restart"},
    {pleroma::admin_config, "/api/pleroma/admin/config"},
    {pleroma::admin_config_descriptions,
     "/api/pleroma/admin/config/descriptions"},
    {pleroma::admin_moderation_log, "/api/pleroma/admin/moderation_log"},
    {pleroma::admin_reload_emoji, "/api/pleroma/admin/reload_emoji"},
    {pleroma::admin_users_confirm_email,
     "/api/pleroma/admin/users/confirm_email"},
    {pleroma::admin_users_resend_confirm_email,
     "/api/pleroma/admin/users/resend_confirm_email"},
    {pleroma::admin_stats, "/api/pleroma/admin/stats"},

    {pleroma::admin_users_nickname, "/api/pleroma/admin/users/<NICKNAME>"},
    {pleroma::admin_users_nickname_activation_status,
     "/api/pleroma/admin/users/<NICKNAME>/activation_status"},
    {pleroma::admin_reports_id_respond,
     "/api/pleroma/admin/reports/<ID>/respond"},
    {pleroma::admin_config_migrate_to_db,
     "/api/pleroma/admin/config/migrate_to_db"},
    {pleroma::admin_config_migrate_from_db,
     "/api/pleroma/admin/config/migrate_from_db"},

    {pleroma::emoji, "/api/pleroma/emoji"},
    {pleroma::follow_import, "/api/pleroma/follow_import"},
    {pleroma::captcha, "/api/pleroma/captcha,"},
    {pleroma::delete_account, "/api/pleroma/delete_account"},
    {pleroma::disable_account, "/api/pleroma/disable_account"},
    {pleroma::notification_settings, "/api/pleroma/notification_settings"},
    {pleroma::healthcheck, "/api/pleroma/healthcheck"},
    {pleroma::change_email, "/api/pleroma/change_email"},
    {pleroma::emoji_packs, "/api/pleroma/emoji/packs"},
    {pleroma::emoji_packs_name, "/api/pleroma/emoji/packs/<NAME>"},
    {pleroma::emoji_packs_name_update_file,
     "/api/pleroma/emoji/packs/<NAME>/update_file"},
    {pleroma::emoji_packs_name_update_metadata,
     "/api/pleroma/emoji/packs/<NAME>/update_metadata"},
    {pleroma::emoji_packs_download_from,
     "/api/pleroma/emoji/packs/download_from"},
    {pleroma::emoji_packs_list_from, "/api/pleroma/emoji/packs/list_from"},
    {pleroma::emoji_packs_name_download_shared,
     "/api/pleroma/emoji/packs/<NAME>/download_shared"},

    {pleroma::account_register, "/api/pleroma/account/register"},
};

} // namespace mastodonpp
