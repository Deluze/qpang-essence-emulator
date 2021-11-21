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
#include "trading/HandleTradeRequest.h"
#include "trading/HandleUpdateTradeStateRequest.h"
#include "training/HandleTrainingRequest.h"

LobbyPacketHandler::LobbyPacketHandler() : PacketHandler()
{
	add(1, new HandleHandshakeRequest());
	add(600, new HandleLoginRequest());
	add(620, new HandleEquipArmorRequest());
	add(623, new HandleEquipWeaponRequest());
	add(646, new HandleGetEquippedSkillCardsRequest());
	add(649, new HandleUpdateSkillSetRequest());
	add(652, new HandleDeleteInventoryCardRequest());
	add(655, new HandleOpenInventoryCardRequest());
	add(667, new HandleValidateNickNameRequest());
	// 670 - Choose nickname and characters, create account.
	add(670, new HandleAccountRegistrationRequest());
	// 767 - Validate/check friend referral.
	add(676, new HandleReferralRegistrationRequest());
	add(679, new HandleChangePlayerCharacterRequest());
	add(691, new HandleGetPlayerInfoRequest());
	add(694, new HandleRequestFriendList());
	add(697, new HandleSendFriendRequest());
	add(701, new HandleAcceptIncomingFriendRequest());
	add(705, new HandleDenyIncomingFriendRequest());
	add(709, new HandleCancelOutgoingFriendRequest());
	add(713, new HandleRemoveFriendRequest());
	// 717 - Invite friend to join the gameroom
	add(721, new HandleGameRoomInviteRequest());
	add(725, new HandleGetMemosRequest());
	// 728 - Send memo
	add(738, new HandlePlayerWhisperRequest());
	add(742, new HandleOpenGiftRequest());
	add(745, new HandleGetGiftsRequest());
	// 751 - Has something to do with joining a room (also occurs with creating a room)
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
	add(812, new HandleGiftCardRequest());
	add(815, new HandleSendGiftInShopRequest());
	// 826 = Trigger NTS_ROY_NEWBIE_EVENT_DESC_START_1 (outgoing)
	add(828, new HandleTrainingRequest());
	add(831, new HandleGetPlayerCashBalanceRequest());
	add(834, new HandleEnableFunctionCardRequest());
	add(837, new HandleChangeNickNameRequest());
	add(841, new HandleResetPlayerWinLossRequest());
	add(844, new HandleResetPlayerKillDeathRequest());
	add(851, new HandleUseRedeemCodeRequest());
	add(861, new HandleDisableFunctionCardRequest());
	
	add(875, new HandleTradeRequest());
	add(879, new HandleReceiveTradeRequest());
	add(884, new HandleUpdateTradeStateRequest());

	add(897, new HandleUseCraneRequest());
	add(901, new HandleGetDailyBonusRequest());
	add(903, new HandleEnchantItemRequest());
	// 903 - Request to boost clothing piece
	// 906 - Panthalassa box opening

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
