#pragma once
	
class SendOpenTradeWindow : public LobbyServerPacket
{
public:
	explicit SendOpenTradeWindow(const uint32_t targetPlayerId, const uint8_t hasAcceptedTradeRequest) : LobbyServerPacket(883)
	{
		writeInt(targetPlayerId);
		writeByte(hasAcceptedTradeRequest);
	}
};