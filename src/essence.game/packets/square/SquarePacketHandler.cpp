#include "SquarePacketHandler.h"

#include "core/communication/client/QpangConnection.h"

#include "qpang/player/Player.h"

#include "packets/square/incoming/Handshake.h"
#include "packets/square/incoming/ConnectRequest.h"
#include "packets/square/incoming/JoinSquare.h"
#include "packets/square/incoming/RequestPlayers.h"
#include "packets/square/incoming/UpdatePosition.h"
#include "packets/square/incoming/LeftInventory.h"
#include "packets/square/incoming/ChatRequest.h"
#include "packets/square/incoming/EmoteEvent.h"
#include "packets/square/incoming/UpdateStateEvent.h"
#include "packets/square/incoming/ReloadSquareEvent.h"

SquarePacketHandler::SquarePacketHandler() : PacketHandler()
{
	add(1, new Handshake());

	add(6500, new ConnectRequest());
	add(6537, new JoinSquare());
	add(6506, new RequestPlayers());
	add(6510, new UpdatePosition());
	add(6514, new LeftInventory());
	add(6526, new ChatRequest());
	add(6530, new ReloadSquareEvent());
	add(6544, new UpdateStateEvent());
	add(6557, new EmoteEvent());
}

void SquarePacketHandler::handle(QpangConnection::Ptr conn, QpangPacket packet)
{
	auto it = m_events.find(packet.getPacketId());//m_events[packet.getPacketId()];

	if (it == m_events.cend())
		return;

	try
	{
		it->second->handle(conn, packet);
	}
	catch (const std::exception& e)
	{
		std::cout << "[PacketHandler::handle] An exception occurred handling ID: " << packet.getPacketId() << " -- " << e.what() << std::endl;
	}
}
