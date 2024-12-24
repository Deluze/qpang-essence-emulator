#pragma once

class SendTradeCancelSelf : public LobbyServerPacket
{
public:
	explicit SendTradeCancelSelf(const uint32_t targetPlayerId, const uint8_t state) : LobbyServerPacket(885)
	{
		writeInt(targetPlayerId);
		writeByte(state);
	}
};