#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/channel/ChannelManager.h"

#include "packets/lobby/outgoing/channel/ChannelHost.h"

class RequestChannelHost : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		uint32_t channelId = packet.readInt();

		Channel ch = Game::instance()->getChannelManager()->getChannel(channelId);

		conn->send(ChannelHost(ch));
	}
};