#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "qpang/channel/Channel.h"

class ChannelList : public LobbyServerPacket
{
public:
	explicit ChannelList(const std::vector<Channel>& channels) : LobbyServerPacket(763)
	{
		const auto size = static_cast<uint16_t>(channels.size());

		writeShort(size);
		writeShort(size);
		writeShort(size);

		// ReSharper disable once CppUseStructuredBinding
		for (const Channel& channel : channels)
		{
			writeShort(channel.id);
			writeString(channel.name, 30);
			writeByte(channel.minLevel);
			writeByte(channel.maxLevel);
			writeShort(channel.currPlayers);
			writeShort(channel.maxPlayers);
			writeEmpty(51);
			writeByte(0);
		}
	}
};