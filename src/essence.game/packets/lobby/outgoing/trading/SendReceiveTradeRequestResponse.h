#pragma once

class SendReceiveTradeRequestResponse : public LobbyServerPacket
{
public:
	explicit SendReceiveTradeRequestResponse(const uint32_t targetPlayerId, const uint8_t hasAcceptedTradeRequest) : LobbyServerPacket(882)
	{
		writeInt(targetPlayerId);
		writeByte(hasAcceptedTradeRequest);
	}
};