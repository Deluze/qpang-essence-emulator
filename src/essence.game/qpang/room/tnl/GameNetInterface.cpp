#include "GameNetInterface.h"

GameNetInterface::GameNetInterface(const TNL::Address& bindAddress) 
	: TNL::NetInterface(bindAddress)
{
	setAllowsConnections(true);
}

void GameNetInterface::processPacket(const TNL::Address& address, TNL::BitStream* packetStream)
{
	TNL::NetInterface::processPacket(address, packetStream);
}

void GameNetInterface::handleInfoPacket(const TNL::Address& address, TNL::U8 packetType, TNL::BitStream* stream)
{
	switch (packetType)
	{
		//Info handshake.
	case PacketType::FirstValidInfoPacketId: {
		TNL::PacketStream pingResponse;

        pingResponse.write(TNL::U8(9));

		pingResponse.writeInt(5, 32);
		pingResponse.writeInt(6, 32);

        pingResponse.sendto(mSocket, address);
		break;
	}
	}
}
