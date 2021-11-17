#pragma once

#include "core/Emulator.h"
#include "core/config/ConfigManager.h"

#include "packets/LobbyServerPacket.h"
#include "qpang/channel/Channel.h"

class ChannelHost : public LobbyServerPacket
{
public:
	explicit ChannelHost(const Channel& ch) : LobbyServerPacket(767)
	{
		writeInt(ch.id);
		writeLong(0);
		// ReSharper disable once CppDeprecatedEntity
		writeInt(inet_addr(CONFIG_MANAGER->getString("HOST").c_str()));
		writeInt(0);
		writeInt(0);
		writeInt(0);
	}
};