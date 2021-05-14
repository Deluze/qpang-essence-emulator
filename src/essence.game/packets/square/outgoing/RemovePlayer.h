#pragma once

#include "packets/SquareServerPacket.h"

#include "qpang/player/Player.h"
#include "qpang/player/equipment/EquipmentManager.h"
#include "qpang/square/SquarePlayer.h"

class RemovePlayer : public SquareServerPacket
{
public:
	RemovePlayer(uint32_t playerId) : SquareServerPacket(6509)
	{
		writeInt(playerId);
	}
};