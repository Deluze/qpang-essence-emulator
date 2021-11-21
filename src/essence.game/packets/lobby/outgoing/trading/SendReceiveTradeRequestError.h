#pragma once

class SendReceiveTradeRequestError : public LobbyServerPacket
{
public:
	explicit SendReceiveTradeRequestError(const uint32_t errorCode) : LobbyServerPacket(877)
	{
		writeInt(errorCode);
	}
};