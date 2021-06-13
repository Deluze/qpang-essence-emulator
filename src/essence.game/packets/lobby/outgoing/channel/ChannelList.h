#pragma once

#include <vector>

#include "qpang/channel/Channel.h"
#include "packets/LobbyServerPacket.h"

class ChannelList : public LobbyServerPacket
{
public:
	ChannelList(const std::vector<Channel>& channels) : LobbyServerPacket(763)
	{
		uint16_t size = static_cast<uint16_t>(channels.size());

		writeShort(size);
		writeShort(size);
		writeShort(size);

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