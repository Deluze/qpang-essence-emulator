#pragma once

#include <string>
#include <cstdint>

#include "core/Emulator.h"
#include "core/config/ConfigManager.h"

#include "core/communication/packet/PacketEvent.h"
#include "GameRoomInviteResponse.h"

class GameRoomInviteRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto playerName = conn->getPlayer()->getName();

		// 38 empty bytes.
		packet.readEmpty(38);

		// TODO: Compare the given host and port from the client to the actual host and port.
		const auto clientHost = inet_addr(readHost(packet).c_str());
		const auto clientPort = packet.readShort();

		// 6 bytes, 1 0 0 0 0 0
		packet.readEmpty(6);

		const auto mode = (uint8_t) packet.readByte();

		const auto roomTitle = packet.readStringTerminated(20);

		// 11 unknown leftover bytes, 1st byte always 0, last 8 bytes remain the same (68 0 246 20 60 43 4 0).
		packet.readEmpty(11);

		const auto host = CONFIG_MANAGER->getString("HOST");
		const auto port = static_cast<uint16_t>(CONFIG_MANAGER->getInt("GAME_PORT"));

		for (const auto& [id, player] : Game::instance()->getPlayers())
		{
			if (const auto squarePlayer = player->getSquarePlayer(); squarePlayer != nullptr)
			{
				squarePlayer->getPlayer()->send(GameRoomInviteResponse(inet_addr(host.c_str()), port, mode, roomTitle));
			}
		}
	}
private:
	std::string readHost(QpangPacket& packet)
	{
		std::array<std::byte, 4> hostAddressParts = packet.readArray<std::byte, 4>();

		std::ostringstream stream;

		for (int i = 0; i < sizeof(hostAddressParts) / sizeof(hostAddressParts[0]); ++i)
		{
			if (i)
			{
				stream << '.';
			}

			stream << (int)hostAddressParts[i];
		}

		return stream.str();
	}
};