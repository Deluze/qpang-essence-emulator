#pragma once

#include "packets/SquareServerPacket.h"

#include "qpang/player/Player.h"
#include "qpang/square/SquarePlayer.h"

class UpdatePlayerEquipment : public SquareServerPacket
{
public:
	explicit UpdatePlayerEquipment(const SquarePlayer::Ptr squarePlayer) : SquareServerPacket(6517)
	{
		const auto player = squarePlayer->getPlayer();

		writeInt(player->getId());
		writeShort(player->getCharacter());
		writeInt(squarePlayer->getSelectedWeapon());
		writeArray<uint32_t, 9>(player->getEquipmentManager()->getArmorItemIdsByCharacter(player->getCharacter()));
	}
};