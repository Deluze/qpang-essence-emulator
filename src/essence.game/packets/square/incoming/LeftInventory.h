#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"
#include "qpang/square/SquarePlayer.h"

class LeftInventory : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		const auto idk = packet.readInt();
		const auto character = packet.readShort();
		const auto selectedWeapon = packet.readInt();
		const auto equipment = packet.readArray<uint32_t, 9>();

		const auto player = conn->getPlayer();

		if (player == nullptr)
			return;

		const auto squarePlayer = player->getSquarePlayer();

		if (squarePlayer == nullptr)
			return;
		
		squarePlayer->changeWeapon(selectedWeapon);
	}
};