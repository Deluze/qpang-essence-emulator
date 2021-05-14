#pragma once

#include <memory>

#include "core/communication/packet/ServerPacket.h"

#include "qpang/player/Player.h"
#include "qpang/square/SquarePlayer.h"
#include "qpang/room/Room.h"
#include "qpang/room/RoomPlayer.h"

class SquarePlayerWriter
{
public:
	static void write(ServerPacket* packet, const std::shared_ptr<SquarePlayer>& squarePlayer)
	{
		const auto player = squarePlayer->getPlayer();

		packet->writeInt(squarePlayer->getState()); // 4
		packet->writeInt(player->getId()); // 8
		packet->writeString(player->getName(), 16); // 40
		packet->writeByte(player->getLevel()); // 41
		packet->writeByte(player->getRank()); // 42
		packet->writeShort(0); // 44
		packet->writeShort(player->getCharacter()); // 46
		packet->writeInt(squarePlayer->getSelectedWeapon()); // 50

		auto armor = player->getEquipmentManager()->getArmorItemIdsByCharacter(player->getCharacter());
		packet->writeArray<uint32_t, 9>(armor); // 86
		packet->writeEmpty(2, 0); // 88
		packet->writeArray<float, 3>(squarePlayer->getPosition()); // 100
	}
};