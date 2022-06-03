#include "LobbyPacketHandler.h"

#include "account/HandleAccountRegistrationRequest.h"
#include "account/HandleReferralRegistrationRequest.h"
#include "channel/HandleRequestChannelHost.h"
#include "channel/HandleRequestChannelList.h"
#include "crane/HandleUseCraneRequest.h"
#include "dailybonus/HandleGetDailyBonusRequest.h"
#include "enchanting/HandleEnchantItemRequest.h"
#include "equipment/HandleEquipArmorRequest.h"
#include "equipment/HandleEquipWeaponRequest.h"
#include "equipment/HandleGetEquippedSkillCardsRequest.h"
#include "equipment/HandleUpdateSkillSetRequest.h"
#include "friend/HandleAcceptIncomingFriendRequest.h"
#include "friend/HandleCancelOutgoingFriendRequest.h"
#include "friend/HandleDenyIncomingFriendRequest.h"
#include "friend/HandleRemoveFriendRequest.h"
#include "friend/HandleRequestFriendList.h"
#include "friend/HandleSendFriendRequest.h"
#include "gameroom/HandleGameRoomInviteRequest.h"
#include "gameroom/HandleGetGameRoomsRequest.h"
#include "gameroom/HandleGetGameSettingsRequest.h"
#include "HandleHandshakeRequest.h"
#include "HandleLoginRequest.h"
#include "HandleOpenLootBoxRequest.h"
#include "inventory/HandleDeleteInventoryCardRequest.h"
#include "inventory/HandleDisableFunctionCardRequest.h"
#include "inventory/HandleEnableFunctionCardRequest.h"
#include "inventory/HandleGetGiftsRequest.h"
#include "inventory/HandleGetInventoryRequest.h"
#include "inventory/HandleGiftCardRequest.h"
#include "inventory/HandleOpenGiftRequest.h"
#include "inventory/HandleOpenInventoryCardRequest.h"
#include "inventory/HandleRenewInventoryCardRequest.h"
#include "memo/HandleGetMemosRequest.h"
#include "nickname/HandleChangeNickNameRequest.h"
#include "nickname/HandleValidateNickNameRequest.h"
#include "player/HandleChangePlayerCharacterRequest.h"
#include "player/HandleGetPlayerCashBalanceRequest.h"
#include "player/HandleGetPlayerInfoRequest.h"
#include "player/HandleGetPlayerRankingRequest.h"
#include "player/HandlePlayerWhisperRequest.h"
#include "player/HandleResetPlayerKillDeathRequest.h"
#include "player/HandleResetPlayerWinLossRequest.h"
#include "redeemcode/HandleUseRedeemCodeRequest.h"
#include "shop/HandleBuyShopCardRequest.h"
#include "shop/HandleGetShopItemsRequest.h"
#include "shop/HandleGetShopPackagesRequest.h"
#include "shop/HandleSendGiftInShopRequest.h"
#include "trading/HandleReceiveTradeRequest.h"
#include "trading/HandleTradeAddCardRequest.h"
#include "trading/HandleTradeRequest.h"
#include "trading/HandleUpdateTradeStateRequest.h"
#include "training/HandleTrainingRequest.h"

LobbyPacketHandler::LobbyPacketHandler() : PacketHandler()
{
	add(1, new HandleHandshakeRequest());
	add(600, new HandleLoginRequest());
	add(620, new HandleEquipArmorRequest());
	// PacketId = 622, Direction: SEND, Description: "Your level is too high for this item."
	add(623, new HandleEquipWeaponRequest());
	add(646, new HandleGetEquippedSkillCardsRequest());
	add(649, new HandleUpdateSkillSetRequest());
	add(652, new HandleDeleteInventoryCardRequest());
	add(655, new HandleOpenInventoryCardRequest());
	// TODO: Re-enable packet when we going to use this feature.
	//add(667, new HandleValidateNickNameRequest());
	// 670 - Choose nickname and characters, create account.
	// TODO: Re-enable packet when we going to use this feature.
	//add(670, new HandleAccountRegistrationRequest());
	// 767 - Validate/check friend referral.
	add(676, new HandleReferralRegistrationRequest());
	add(679, new HandleChangePlayerCharacterRequest());
	add(691, new HandleGetPlayerInfoRequest());
	// 693, Direction: SEND, Description: "Notice: Message:GET_OTHER_INFO_FAIL"
	add(694, new HandleRequestFriendList());
	add(697, new HandleSendFriendRequest());
	// 699, Direction: SEND, Description: "LS_ADD_BUDDY_FAIL"
	// 700, Direction: SEND, Description: "Shows notification with: 'x asks for a friend'"
	add(701, new HandleAcceptIncomingFriendRequest());
	// 703, Direction: SEND, Description: "LS_ACCEPT_BUDDY_FAIL"
	add(705, new HandleDenyIncomingFriendRequest());
	// 707, Direction: SEND, Description: "LS_DENY_BUDDY_FAIL"
	// 708, Direction: SEND, Description: "Shows notification with 'x' denies becoming your friend"
	add(709, new HandleCancelOutgoingFriendRequest());
	// 711, Direction: SEND, Description: "LS_CANCEL_BUDDY_FAIL"
	// 712, Direction: SEND, Description: "Shows notification with 'x' cancels a friend request for a friend"
	add(713, new HandleRemoveFriendRequest());
	// 717 - Invite friend to join the gameroom
	add(721, new HandleGameRoomInviteRequest());
	add(725, new HandleGetMemosRequest());
	// 728 - Send memo (729: success, 730: error, 731: to target, 737: LS_SEND_MEMO_FAIL)
	add(738, new HandlePlayerWhisperRequest());
	add(742, new HandleOpenGiftRequest());
	add(745, new HandleGetGiftsRequest());
	// 751 - Has something to do with joining a room (also occurs with creating a room)
	// 757, Direction: SEND, Description: "Notice: 'x' refused the invitation"
	add(758, new HandleGetGameRoomsRequest());
	add(762, new HandleRequestChannelList());
	add(766, new HandleRequestChannelHost());
	add(769, new HandleGetGameSettingsRequest());
	add(780, new HandleGetInventoryRequest());
	add(791, new HandleGetPlayerRankingRequest());
	add(797, new HandleGetShopItemsRequest());
	add(800, new HandleGetShopPackagesRequest());
	add(803, new HandleBuyShopCardRequest());
	add(809, new HandleRenewInventoryCardRequest());
	// 810, Direction: SEND, Description: "The duration of the item has been successfully extended to x rounds(hours)"
	// 811, Direction: SEND, Description: "There is an error in process: LS_RENEW_CARD_FAIL"
	add(812, new HandleGiftCardRequest());
	// 814, Direction: SEND, Description: "There is an error in process: LS_SEND_PRESENT_INDEC_FAIL"
	add(815, new HandleSendGiftInShopRequest());
	// 819, Direction: SEND, Description: Opens dialog/notice with a 'You present x to x' text and card image
	// 820, Direction: SEND, Description: "There is an error in process: LS_SEND_PRESENT_INSHOP_FAIL"
	// 821, Direction: SEND, Description: A notification gets send with 'x sends you a gift':
	// 826, Direction: SEND, Description: "A story gets told by ROY and guides the player through the game"
	// 827, Direction: SEND, Description: "There is an error in process: LS_NEWUSER_EVENT_FAIL"
	add(828, new HandleTrainingRequest());
	add(831, new HandleGetPlayerCashBalanceRequest());
	add(834, new HandleEnableFunctionCardRequest());
	add(837, new HandleChangeNickNameRequest());
	add(841, new HandleResetPlayerWinLossRequest());
	add(844, new HandleResetPlayerKillDeathRequest());
	// PacketId = 846, Direction: SEND, Description: "There is an error in process: LS_CLEAR_KILL_DEATH_FAIL"
	// PacketId = 850, Direction: SEND, Description: "You become the referral for YestinJarrett" (String as argument)
	add(851, new HandleUseRedeemCodeRequest());
	// PacketId = 853, Direction: SEND, Description: "This is incorrect/unavailable. Please try again"
	add(861, new HandleDisableFunctionCardRequest());
	
	add(875, new HandleTradeRequest());
	add(879, new HandleReceiveTradeRequest());
	add(884, new HandleUpdateTradeStateRequest());
	add(888, new HandleTradeAddCardRequest());

	add(897, new HandleUseCraneRequest());
	add(901, new HandleGetDailyBonusRequest());
	add(903, new HandleEnchantItemRequest());
	// PacketId = 904, Direction: SEND, Description: Plays a sound?
	// PacketId = 905, Direction: SEND, Description: "This item is not allowed to enchant"
	// PacketId = 906, Direction: RECEIVE, Description: Panthalassa box opening
	add(906, new HandleOpenLootBoxRequest());
	/*
		LOBBY_TRADE_ERROR = 877,
		LOBBY_TRADE_RECEIVE_REQUEST = 878,
		LOBBY_TRADE_REQUEST = 879,
		LOBBY_TRADE_REQUEST_RSP = 882,
		LOBBY_TRADE_OPEN_MENU = 883,
		LOBBY_TRADE_CANCEL = 884,
		LOBBY_TRADE_CANCEL_SELF = 885,
		LOBBY_TRADE_CANCEL_OTHER = 887,
		LOBBY_TRADE_ADD_CARD = 888,
		LOBBY_TRADE_ADD_CARD_SELF = 889,
		LOBBY_TRADE_ADD_CARD_OTHER = 891,
		LOBBY_TRADE_FINISHED = 892,

		_events[Opcode::LOBBY_OPEN_PLAYER_CARD] = new OpenCardEvent();
		_events[Opcode::LOBBY_BUY_ITEM] = new BuyCardEvent();
		_events[Opcode::LOBBY_SHOP_GIFT] = new GiftShopCardEvent();
		_events[Opcode::LOBBY_TRADE] = new LobbyTradeEvent();
		_events[Opcode::LOBBY_TRADE_REQUEST] = new TradeRequestEvent();
		_events[Opcode::LOBBY_TRADE_CANCEL] = new TradeCancelEvent();
		_events[Opcode::LOBBY_TRADE_ADD_CARD] = new TradeAddCardEvent();
		_events[Opcode::LOBBY_PLAYERINFO] = new PlayerInfoEvent();
		_events[Opcode::LOBBY_FRIEND_INVITE] = new AddFriendEvent();
		_events[Opcode::LOBBY_REMOVE_OUTGOING_FRIEND] = new RemoveOutgoingFriendEvent();
	*/
}
