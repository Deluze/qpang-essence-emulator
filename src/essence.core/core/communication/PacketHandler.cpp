#include "PacketHandler.h"

#include "core/communication/client/QpangConnection.h"
#include "core/communication/packet/PacketEvent.h"

void PacketHandler::handle(QpangConnection::Ptr conn, QpangPacket pack)
{
	const auto it = m_events.find(pack.getPacketId());

	if (it == m_events.cend())
	{
//#ifdef WIN32
//		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
//
//		SetConsoleTextAttribute(handle, 12);
//#endif
//		std::cout << "PacketHandler::handle >> Received an UNKNOWN packet with ID " << pack.getPacketId() << std::endl;

		return;
	}

//#ifdef WIN32
//	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
//
//	SetConsoleTextAttribute(handle, 10);
//#endif
//	std::cout << "PacketHandler::handle >> Handling packet with ID " << pack.getPacketId() << std::endl;
//#ifdef WIN32
//	SetConsoleTextAttribute(handle, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
//#endif
	const bool connAuthorized = conn->getPlayer() != nullptr;
	const bool packProtected = !it->second->isUnauthorizedPacket();

	if (connAuthorized && packProtected || !connAuthorized && !packProtected)
	{
		try
		{
			it->second->handle(conn, pack);
		}
		catch (const std::exception& e)
		{
			printf("(PacketHandler::handle) An exception occurred whilst attempting to handle packet id %u.\n", pack.getPacketId());
			printf("(PacketHandler::handle) Corresponding exception: %s.\n", e.what());
		}
	}
}

void PacketHandler::add(uint16_t packetId, PacketEvent* packEvent)
{
	m_events[packetId] = packEvent;
}