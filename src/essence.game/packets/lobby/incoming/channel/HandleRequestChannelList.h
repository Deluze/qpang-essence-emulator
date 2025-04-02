#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "packets/lobby/outgoing/channel/SendChannelList.h"

class HandleRequestChannelList final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const std::vector<Channel> channels = Game::instance()->getChannelManager()->list();

		conn->send(SendChannelList(channels));
	}
};