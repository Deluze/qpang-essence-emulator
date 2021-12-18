#pragma once

class SendTradeFinished : public LobbyServerPacket
{
public:
	explicit SendTradeFinished(const uint32_t targetPlayerId) : LobbyServerPacket(892)
	{
		writeInt(targetPlayerId);
	}
};