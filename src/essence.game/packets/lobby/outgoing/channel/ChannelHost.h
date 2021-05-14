#pragma once

#include <vector>

#include "core/Emulator.h"
#include "core/config/ConfigManager.h"

#include "qpang/channel/Channel.h"
#include "packets/LobbyServerPacket.h"

class ChannelHost : public LobbyServerPacket
{
public:
	ChannelHost(const Channel& ch) : LobbyServerPacket(767)
	{
		writeInt(ch.id);
		writeLong(0);
		writeInt(inet_addr(CONFIG_MANAGER->getString("HOST").c_str()));
		writeInt(0);
		writeInt(0);
		writeInt(0);
	}
};