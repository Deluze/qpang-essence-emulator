#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "packets/lobby/outgoing/channel/ChannelList.h"

class RequestChannelList : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		std::vector<Channel> channels = Game::instance()->getChannelManager()->list();

		conn->send(ChannelList(channels));
	}
};