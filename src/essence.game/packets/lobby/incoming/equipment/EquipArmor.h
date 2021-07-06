#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"
#include "qpang/player/equipment/EquipmentManager.h"

class EquipArmor : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		const auto characterIndex = packet.readShort();

		std::array<uint64_t, 9> armor{};

		for (size_t i = 0; i < 9; i++)
			armor[i] = packet.readLong();

		conn->getPlayer()->getEquipmentManager()->setArmor(characterIndex, armor);
	}
};