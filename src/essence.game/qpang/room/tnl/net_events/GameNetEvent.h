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
};