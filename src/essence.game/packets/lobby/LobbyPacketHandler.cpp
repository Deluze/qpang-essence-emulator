#include "LobbyPacketHandler.h"


#include "GiftCardEvent.h"
#include "UseCraneEvent.h"
#include "packets/lobby/incoming/Handshake.h"
#include "packets/lobby/incoming/Login.h"

#include "packets/lobby/incoming/channel/RequestChannelList.h"
#include "packets/lobby/incoming/channel/RequestChannelHost.h"
#include "packets/lobby/incoming/inventory/RequestInventory.h"
#include "packets/lobby/incoming/equipment/RequestEquippedSkillCards.h"
#include "packets/lobby/incoming/shop/RequestShopItems.h"
#include "packets/lobby/incoming/shop/RequestShopPackages.h"
#include "packets/lobby/incoming/player/RequestCashBalance.h"
#include "packets/lobby/incoming/friend/RequestFriendList.h"
#include "packets/lobby/incoming/player/RequestPlayerInfo.h"
#include "packets/lobby/incoming/memo/RequestMemos.h"
#include "packets/lobby/incoming/player/RequestPlayerRanking.h"
#include "packets/lobby/incoming/inventory/RequestGifts.h"
#include "packets/lobby/incoming/inventory/OpenGift.h"
#include "packets/lobby/incoming/inventory/DeleteCard.h"
#include "packets/lobby/incoming/equipment/EquipWeapon.h"
#include "packets/lobby/incoming/equipment/EquipArmor.h"
#include "packets/lobby/incoming/shop/BuyCardEvent.h"
#include "packets/lobby/incoming/player/ChangeCharacterEvent.h"
#include "packets/lobby/incoming/inventory/EnableFunctionCardEvent.h"
#include "packets/lobby/incoming/inventory/DisableFunctionCardEvent.h"
#include "packets/lobby/incoming/inventory/ExtendCardEvent.h"
#include "packets/lobby/incoming/friend/SendFriendRequestEvent.h"
#include "packets/lobby/incoming/friend/DenyIncomingFriendRequestEvent.h"
#include "packets/lobby/incoming/friend/CancelOutgoingFriendRequestEvent.h"
#include "packets/lobby/incoming/friend/AcceptIncomingFriendRequestEvent.h"
#include "packets/lobby/incoming/friend/RemoveFriendEvent.h"
#include "packets/lobby/incoming/player/WhisperEvent.h"
#include "packets/lobby/incoming/gameroom/RequestGameSettingsEvent.h"
#include "packets/lobby/incoming/gameroom/RequestGameRoomsEvent.h"
#include "packets/lobby/incoming/player/ResetWinLossEvent.h"
#include "packets/lobby/incoming/player/ResetKillDeathEvent.h"

LobbyPacketHandler::LobbyPacketHandler() : PacketHandler()
{
	add(1, new Handshake());
	add(600, new Login());
	add(620, new EquipArmor());
	add(623, new EquipWeapon());
	add(646, new RequestEquippedSkillCards());
	add(652, new DeleteCard());
	add(679, new ChangeCharacterEvent());
	add(691, new RequestPlayerInfo());
	add(694, new RequestFriendList());
	add(697, new SendFriendRequestEvent());
	add(701, new AcceptIncomingFriendRequestEvent());
	add(705, new DenyIncomingFriendRequestEvent());
	add(709, new CancelOutgoingFriendRequestEvent());
	add(713, new RemoveFriendEvent());
	add(725, new RequestMemos());
	add(738, new WhisperEvent());
	add(742, new OpenGift());
	add(745, new RequestGifts());
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
	add(831, new RequestCashBalance());
	add(834, new EnableFunctionCardEvent());
	add(841, new ResetWinLossEvent());
	add(844, new ResetKillDeathEvent());
	add(861, new DisableFunctionCardEvent());
	add(897, new UseCraneEvent());
}