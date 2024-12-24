#pragma once

class SendTradeRequestSuccess : public LobbyServerPacket
{
public:
	explicit SendTradeRequestSuccess() : LobbyServerPacket(876)
	{
		//writeInt()
	}
};