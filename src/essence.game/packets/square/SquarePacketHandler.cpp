#include "SquarePacketHandler.h"

#include "core/communication/client/QpangConnection.h"
#include "packets/square/incoming/HandleSquarePlayerChatRequest.h"
#include "packets/square/incoming/HandleSquareConnectRequest.h"
#include "packets/square/incoming/HandleSquarePlayerUseEmoteRequest.h"
#include "packets/square/incoming/HandleHandshakeRequest.h"
#include "packets/square/incoming/HandleJoinSquareRequest.h"
#include "packets/square/incoming/HandleLeaveInventoryRequest.h"
#include "packets/square/incoming/HandleReloadSquareRequest.h"
#include "packets/square/incoming/HandleGetSquarePlayersRequest.h"
#include "packets/square/incoming/HandleUpdateSquarePlayerPositionRequest.h"
#include "packets/square/incoming/HandleUpdateSquarePlayerStateRequest.h"

SquarePacketHandler::SquarePacketHandler() : PacketHandler()
{
	add(1, new HandleHandshakeRequest());

	add(6500, new HandleSquareConnectRequest());
	add(6537, new HandleJoinSquareRequest());
	add(6506, new HandleGetSquarePlayersRequest());
	add(6510, new HandleUpdateSquarePlayerPositionRequest());
	add(6514, new HandleLeaveInventoryRequest());
	add(6526, new HandleSquarePlayerChatRequest());
	add(6530, new HandleReloadSquareRequest());
	add(6544, new HandleUpdateSquarePlayerStateRequest());
	add(6557, new HandleSquarePlayerUseEmoteRequest());
}

void SquarePacketHandler::handle(QpangConnection::Ptr conn, QpangPacket packet)
{
	auto it = m_events.find(packet.getPacketId());//m_events[packet.getPacketId()];

	if (it == m_events.cend())
	{
		return;
	}

	try
	{
		it->second->handle(conn, packet);
	}
	catch (const std::exception& e)
	{
		printf("(SquarePacketHandler::handle) An exception occurred whilst attempting to handle square packet id %u.\n", packet.getPacketId());
		printf("(SquarePacketHandler::handle) Corresponding exception: %s.\n", e.what());
	}
}
