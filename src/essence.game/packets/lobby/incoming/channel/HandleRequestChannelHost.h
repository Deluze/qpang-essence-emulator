#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/channel/ChannelManager.h"

#include "packets/lobby/outgoing/channel/SendChannelHost.h"

class HandleRequestChannelHost final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const uint32_t channelId = packet.readInt();

		const Channel ch = Game::instance()->getChannelManager()->getChannel(channelId);

		conn->send(SendChannelHost(ch));
	}
};