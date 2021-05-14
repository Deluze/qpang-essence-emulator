#include "PacketHandler.h"

#include "core/communication/client/QpangConnection.h"
#include "core/communication/packet/PacketEvent.h"

void PacketHandler::handle(QpangConnection::Ptr conn, QpangPacket pack)
{
	const auto it = m_events.find(pack.getPacketId());

	if (it == m_events.cend())
		return;

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
			std::cout << "PacketHandler::handle ID: " << pack.getPacketId() << " -- " << e.what() << std::endl;
		}
	}
}

void PacketHandler::add(uint16_t packetId, PacketEvent* packEvent)
{
	m_events[packetId] = packEvent;
}