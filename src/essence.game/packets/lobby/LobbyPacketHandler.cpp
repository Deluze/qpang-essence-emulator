#include "LobbyPacketHandler.h"

#include "GiftCardEvent.h"
#include "Handshake.h"
#include "Login.h"
#include "channel/RequestChannelHost.h"
#include "channel/RequestChannelList.h"
#include "crane/UseCraneEvent.h"
#include "dailybonus/DailyBonusRequest.h"
#include "equipment/EquipArmor.h"
#include "equipment/EquipWeapon.h"
#include "equipment/RequestEquippedSkillCards.h"
#include "equipment/UpdateSkillCardSetRequest.h"
#include "friend/AcceptIncomingFriendRequestEvent.h"
#include "friend/CancelOutgoingFriendRequestEvent.h"
#include "friend/DenyIncomingFriendRequestEvent.h"
#include "friend/RemoveFriendEvent.h"
#include "friend/RequestFriendList.h"
#include "friend/SendFriendRequestEvent.h"
#include "gameroom/GameRoomInviteRequest.h"
#include "gameroom/RequestGameRoomsEvent.h"
#include "gameroom/RequestGameSettingsEvent.h"
#include "inventory/DeleteCard.h"
#include "inventory/DisableFunctionCardEvent.h"
#include "inventory/EnableFunctionCardEvent.h"
#include "inventory/ExtendCardEvent.h"
#include "inventory/OpenCardRequest.h"
#include "inventory/OpenGift.h"
#include "inventory/RequestGifts.h"
#include "inventory/RequestInventory.h"
#include "memo/RequestMemos.h"
#include "nickname/ChangeNickNameRequest.h"
#include "nickname/ValidateNickNameRequest.h"
#include "player/ChangeCharacterEvent.h"
#include "player/RequestCashBalance.h"
#include "player/RequestPlayerInfo.h"
#include "player/RequestPlayerRanking.h"
#include "player/ResetKillDeathEvent.h"
#include "player/ResetWinLossEvent.h"
#include "player/WhisperEvent.h"
#include "redeemcode/UseRedeemCodeRequest.h"
#include "shop/BuyCardEvent.h"
#include "shop/RequestShopItems.h"
#include "shop/RequestShopPackages.h"
#include "shop/SendPresentInShopRequest.h"
#include "training/TrainingRequest.h"

LobbyPacketHandler::LobbyPacketHandler() : PacketHandler()
{
	add(1, new Handshake());
	add(600, new Login());
	add(620, new EquipArmor());
	add(623, new EquipWeapon());
	add(646, new RequestEquippedSkillCards());
	add(649, new UpdateSkillCardSetRequest());
	add(652, new DeleteCard());
	add(667, new ValidateNickNameRequest());
	add(655, new OpenCardRequest());
	add(679, new ChangeCharacterEvent());
	add(691, new RequestPlayerInfo());
	add(694, new RequestFriendList());
	add(697, new SendFriendRequestEvent());
	add(701, new AcceptIncomingFriendRequestEvent());
	add(705, new DenyIncomingFriendRequestEvent());
	add(709, new CancelOutgoingFriendRequestEvent());
	add(713, new RemoveFriendEvent());
	// 717 - Invite friend to join the gameroom
	add(721, new GameRoomInviteRequest());
	add(725, new RequestMemos());
	// 728 - Send memo
	add(738, new WhisperEvent());
	add(742, new OpenGift());
	add(745, new RequestGifts());
	// 751 - Has something to do with joining a room (also occurs with creating a room)
	add(758, new RequestGameRoomsEvent());
	add(762, new RequestChannelList());
	add(766, new RequestChannelHost());
	add(769, new RequestGameSettingsEvent());
	add(780, new RequestInventory());
	add(791, new RequestPlayerRanking());
	add(797, new RequestShopItems());
	add(800, new RequestShopPackages());
	add(803, new BuyCardEvent());
	add(809, new ExtendCardEvent());
	add(812, new GiftCardEvent());
	add(815, new SendPresentInShopRequest());
	// 826 = Trigger NTS_ROY_NEWBIE_EVENT_DESC_START_1 (outgoing)
	add(828, new TrainingRequest());
	add(831, new RequestCashBalance());
	add(834, new EnableFunctionCardEvent());
	add(837, new ChangeNickNameRequest());
	add(841, new ResetWinLossEvent());
	add(844, new ResetKillDeathEvent());
	add(851, new UseRedeemCodeRequest());
	add(861, new DisableFunctionCardEvent());
	// 875 - Send trade request
	add(897, new UseCraneEvent());
	add(901, new DailyBonusRequest());
	// 903 - Request to boost clothing piece
	// 906 - Panthalassa box opening
}
