#pragma once

#include <string>
#include <cstdint>

#include "core/Emulator.h"
#include "core/config/ConfigManager.h"
#include "core/communication/packet/PacketEvent.h"

#include "packets/lobby/outgoing/gameroom/UpdateGameSettings.h"
#include "packets/lobby/outgoing/gameroom/RoomList.h"

class RequestGameSettingsEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		auto host = CONFIG_MANAGER->getString("HOST");
		auto port = static_cast<uint16_t>(CONFIG_MANAGER->getInt("GAME_PORT"));
		auto isEnabled = CONFIG_MANAGER->getString("GAME_ENABLED") == "1";

		auto rooms = Game::instance()->getRoomManager()->list();

		conn->send(RoomList(rooms));
		conn->send(UpdateGameSettings(inet_addr(host.c_str()), port, isEnabled));
	}
};