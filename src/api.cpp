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

API::API()
    : _endpoint_map
{
    {v1::apps, "/api/v1/apps"},
    {v1::apps_verify_credentials, "/api/v1/apps/verify/credentials"},

    {v1::accounts, "/api/v1/accounts"},
    {v1::accounts_verify_credentials, "/api/v1/accounts/verify/credentials"},
    {v1::accounts_update_credentials, "/api/v1/accounts/update/credentials"},
    {v1::accounts_id, "/api/v1/accounts/id"},
    {v1::accounts_id_statuses, "/api/v1/accounts/<ID>/statuses"},
    {v1::accounts_id_followers, "/api/v1/accounts/<ID>/followers"},
    {v1::accounts_id_following, "/api/v1/accounts/<ID>/following"},
    {v1::accounts_id_lists, "/api/v1/accounts/<ID>/lists"},
    {v1::accounts_id_identity_proofs, "/api/v1/accounts/<ID>/identity/proofs"},
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

    {v1::domain_blocks, "/api/v1/domain/blocks"},

    {v1::filters, "/api/v1/filters"},
    {v1::filters_id, "/api/v1/filters/id"},

    {v1::reports, "/api/v1/reports"},

    {v1::follow_requests, "/api/v1/follow/requests"},
    {v1::follow_requests_id_authorize,
     "/api/v1/follow/requests/<ID>/authorize"},
    {v1::follow_requests_id_reject, "/api/v1/follow/requests/<ID>/reject"},

    {v1::endorsements, "/api/v1/endorsements"},

    {v1::featured_tags, "/api/v1/featured/tags"},
    {v1::featured_tags_id, "/api/v1/featured/tags/id"},
    {v1::featured_tags_suggestions, "/api/v1/featured/tags/suggestions"},

    {v1::preferences, "/api/v1/preferences"},

    {v1::suggestions, "/api/v1/suggestions"},
    {v1::suggestions_account_id, "/api/v1/suggestions/account/id"},

    {v1::statuses, "/api/v1/statuses"},
    {v1::statuses_id, "/api/v1/statuses/id"},
    {v1::statuses_id_context, "/api/v1/statuses/<ID>/context"},
    {v1::statuses_id_reblogged_by, "/api/v1/statuses/<ID>/reblogged/by"},
    {v1::statuses_id_favourited_by, "/api/v1/statuses/<ID>/favourited/by"},
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
    {v1::media_id, "/api/v1/media/id"},

    {v1::polls_id, "/api/v1/polls/id"},
    {v1::polls_id_votes, "/api/v1/polls/<ID>/votes"},

    {v1::scheduled_statuses, "/api/v1/scheduled/statuses"},
    {v1::scheduled_statuses_id, "/api/v1/scheduled/statuses/id"},

    {v1::timelines_public, "/api/v1/timelines/public"},
    {v1::timelines_tag_hashtag, "/api/v1/timelines/tag/hashtag"},
    {v1::timelines_home, "/api/v1/timelines/home"},
    {v1::timelines_list_list_id, "/api/v1/timelines/list/list/id"},

    {v1::conversations, "/api/v1/conversations"},
    {v1::conversations_id, "/api/v1/conversations/id"},
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
    {v1::notifications_id, "/api/v1/notifications/id"},
    {v1::notifications_clear, "/api/v1/notifications/clear"},
    {v1::notifications_id_dismiss, "/api/v1/notifications/<ID>/dismiss"},

    {v1::push_subscription, "/api/v1/push/subscription"},

    {v1::instance, "/api/v1/instance"},
    {v1::instance_peers, "/api/v1/instance/peers"},
    {v1::instance_activity, "/api/v1/instance/activity"},

    {v1::trends, "/api/v1/trends"},

    {v1::directory, "/api/v1/directory"},

    {v1::custom_emojis, "/api/v1/custom/emojis"},

    {v1::admin_accounts, "/api/v1/admin/accounts"},
    {v1::admin_accounts_id, "/api/v1/admin/accounts/id"},
    {v1::admin_accounts_account_id_action,
     "/api/v1/admin/accounts/account/<ID>/action"},
    {v1::admin_accounts_id_approve, "/api/v1/admin/accounts/<ID>/approve"},
    {v1::admin_accounts_id_reject, "/api/v1/admin/accounts/<ID>/reject"},
    {v1::admin_accounts_id_enable, "/api/v1/admin/accounts/<ID>/enable"},
    {v1::admin_accounts_id_unsilence, "/api/v1/admin/accounts/<ID>/unsilence"},
    {v1::admin_accounts_id_unsuspend, "/api/v1/admin/accounts/<ID>/unsuspend"},
    {v1::admin_reports, "/api/v1/admin/reports"},
    {v1::admin_reports_id, "/api/v1/admin/reports/id"},
    {v1::admin_reports_id_assign_to_self,
     "/api/v1/admin/reports/<ID>/assign/to/self"},
    {v1::admin_reports_id_unassign, "/api/v1/admin/reports/<ID>/unassign"},
    {v1::admin_reports_id_resolve, "/api/v1/admin/reports/resolve"},
    {v1::admin_reports_id_reopen, "/api/v1/admin/reports/<ID>/reopen"},

    {v2::search, "/api/v2/search"},

    {oauth::authorize, "/oauth/authorize"},
    {oauth::token, "/oauth/token"},
    {oauth::revoke, "/oauth/revoke"},

    {other::proofs, "/api/proofs"},
    {other::oembed, "/api/oembed"}
}
{}

} // namespace mastodonpp
