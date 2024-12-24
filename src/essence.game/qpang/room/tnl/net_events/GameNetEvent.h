#pragma once

#include <memory>
#include <cstdint>
#include <string>

#include <tnl/tnl.h>
#include <tnl/tnlBitStream.h>
#include <tnl/tnlNetEvent.h>
#include <tnl/tnlByteBuffer.h>

#include "qpang/Game.h"
#include "qpang/room/RoomServer.h"
#include "qpang/room/tnl/net_events/GameNetId.h"

class GameConnection;

using namespace TNL;

class GameNetEvent : public NetEvent
{
public:
	GameNetEvent();
	GameNetEvent(GameNetId gameNetId,
		NetEvent::GuaranteeType guaranteeType,
		NetEvent::EventDirection eventDirection);

	std::u16string byteBufferToString(ByteBuffer* buffer, uint32_t length);

	std::u16string readByteBuffer(BitStream* stream, uint32_t length = 254);
	void writeByteBuffer(BitStream* stream, const std::u16string& string, uint32_t length = 254);

	void notifyPosted(EventConnection* ps);
	void notifyDelivered(EventConnection* ps);

	virtual void handle(GameConnection* conn, Player::Ptr player);

	template<typename Event>
	void post(EventConnection* ps)
	{
		this->gameConnection = reinterpret_cast<GameConnection*>(ps);

		Game::instance()->getRoomServer()->handleEvent(this);
	}

	GameNetId id;
	GameConnection* gameConnection;

	std::map<int, std::string> guaranteeTypeMapping = {
		{0, "GuaranteedOrdered"},
		{1, "Guaranteed"},
		{2, "Unguaranteed"}
	};

	std::map<int, std::string> eventDirectionMapping = {
		{0, "DirUnset"},
		{1, "DirAny"},
		{2, "DirServerToClient"},
		{3, "DirClientToServer"}
	};

	std::map<int, std::string> gameNetIdMapping = {
		{174, "CC_CHARM"},
		{176, "CC_USERINFO"},

		{145, "CG_ARRANGED_COMPLETE"},
		{143, "CG_ARRANGED_REJECT"},
		{154, "CG_AUTH"},
		{147, "CG_CARD"},
		{172, "CG_CHARM"},
		{149, "CG_DISCONNECT"},
		{166, "CG_ESSENCE"},
		{137, "CG_EXIT"},
		{127, "CG_GAME_ITEM"},
		{129, "CG_GAME_STATE"},
		{109, "CG_HIT"},
		{184, "CG_HIT_ESSENCE"},
		{175, "CG_LOG"},
		{177, "CG_MAPOBJECT"},
		{135, "CG_MESG"},
		{164, "CG_MOTION"},
		{105, "CG_MOVE"},
		{181, "CG_MOVE_REPORT"},
		{131, "CG_PLAYER_CHANGE"},
		{532, "CG_PVE_AREA_TRIGGER"},
		{524, "CG_PVE_EVENT_OBJECT"},
		{531, "CG_PVE_FLOOR"},
		{512, "CG_PVE_GET_ITEM"},
		{517, "CG_PVE_HIT_N2P"},
		{506, "CG_PVE_HIT_NPC"},
		{510, "CG_PVE_HIT_NPC_TO_OBJECT"},
		{508, "CG_PVE_HIT_OBJECT"},
		{536, "CG_PVE_RESPAWN_REQ"},
		{527, "CG_PVE_ROUND_START"},
		{500, "CG_PVE_START"},
		{121, "CG_READY"},
		{111, "CG_RESPAWN"},
		{133, "CG_ROOM"},
		{117, "CG_ROOM_INFO"},
		{123, "CG_SCORE"},
		{107, "CG_SHOOT"},
		{182, "CG_SHOOTREPORT"},
		{103, "CG_START"},
		{170, "CG_SYNC"},
		{168, "CG_TARGET"},
		{113, "CG_WEAPON"},
		{161, "CS_RTT_REQUEST"},
		{162, "CS_RTT_REPORT"},
		{163, "CS_RTT_RESPONSE"},

		{140, "GC_ARRANGED_CONN"},
		{142, "GC_ARRANGED_ACCEPT"},
		{148, "GC_CARD"},
		{138, "GC_EXIT"},
		{128, "GC_GAME_ITEM"},
		{130, "GC_GAME_STATE"},
		{110, "GC_HIT"},
		{185, "GC_HIT_ESSENCE"},
		{116, "GC_JOIN"},
		{178, "GC_MAPOBJECT"},
		{183, "GC_MASTERLOG"},
		{136, "GC_MESG"},
		{165, "GC_MOTION"},
		{106, "GC_MOVE"},
		{132, "GC_PLAYER_CHANGE"},
		{533, "GC_PVEA_AREA_TRIGGER_INIT"},
		{523, "GC_PVE_DESTROY_OBJECT"},
		{521, "GC_PVE_DESTROY_PARTS"},
		{522, "GC_PVE_DIE_NPC"},
		{529, "GC_PVE_DOOR"},
		{525, "GC_PVE_EVENT_OBJECT"},
		{520, "GC_PVE_HIT_N2N"},
		{518, "GC_PVE_HIT_N2P"},
		{507, "GC_PVE_HIT_NPC"},
		{511, "GC_PVE_HIT_NPC_TO_OBJECT"},
		{509, "GC_PVE_HIT_OBJECT"},
		{504, "GC_PVE_ITEM_INIT"},
		{515, "GC_PVE_MOVE_NODES_NPC"},
		{514, "GC_PVE_MOVE_NPC"},

		{528, "GC_PVE_NEW_ROUND"},
		{505, "GC_PVE_NPC_INIT"},
		{503, "GC_PVE_OBJECT_INIT"},
		{530, "GC_PVE_OBJECT_MOVE"},
		{526, "GC_PVE_ROUND_END"},
		{516, "GC_PVE_SHOOT_N2P"},
		{501, "GC_PVE_START"},
		{502, "GC_PVE_USER_INIT"},
		{534, "GC_PVE_SCORE_RESULT"},
		{535, "GC_PVE_SPECIAL_ATTACK"},
		{159, "GC_QUEST"},
		{174, "GC_QUEST_2"},
		{122, "GC_READY"},
		{112, "GC_RESPAWN"},
		{134, "GC_ROOM"},
		{118, "GC_ROOM_INFO"},
		{124, "GC_SCORE"},
		{125, "GC_SCORE_RESULT"},
		{108, "GC_SHOOT"},
		{104, "GC_START"},
		{171, "GC_SYNC"},
		{169, "GC_TARGET"},
		{114, "GC_WEAPON"},
		{173, "GC_CHARM"},
		{160, "GG_RELOAD"}
	};
};