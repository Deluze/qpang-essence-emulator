#pragma once

#include <cstdint>

#include "core/communication/server/QpangServer.h"
#include "packets/AuthPacketHandler.h"

class Authenticator
{
public:
	Authenticator();

	void onNewConnection(QpangConnection::Ptr conn);
	void onLobbyConnectionClosed(QpangConnection::Ptr conn);
	void onConnectionData(QpangConnection::Ptr conn, QpangPacket packet);

	static bool verify(std::u16string username, std::u16string password);
private:
	QpangServer* m_server;
	AuthPacketHandler* m_packHandler;
};