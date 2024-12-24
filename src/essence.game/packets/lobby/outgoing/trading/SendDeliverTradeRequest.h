#pragma once

class SendDeliverTradeRequest : public LobbyServerPacket
{
public:
	explicit SendDeliverTradeRequest(const uint32_t senderPlayerId) : LobbyServerPacket(878)
	{
		writeInt(senderPlayerId);
		writeInt(senderPlayerId);
	}
};