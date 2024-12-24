#pragma once

class SendTradeCancelOther : public LobbyServerPacket
{
public:
	explicit SendTradeCancelOther(const uint32_t targetPlayerId, const uint8_t state) : LobbyServerPacket(887)
	{
		writeInt(targetPlayerId);
		writeInt(targetPlayerId);
		writeByte(state);
	}
};