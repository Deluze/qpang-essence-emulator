#pragma once

class SendTradeRequestCancel : public LobbyServerPacket
{
public:
	explicit SendTradeRequestCancel() : LobbyServerPacket(881)
	{

	}
};