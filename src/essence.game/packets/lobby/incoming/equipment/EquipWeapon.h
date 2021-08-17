#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"
#include "qpang/player/equipment/EquipmentManager.h"

class EquipWeapon final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto characterIndex = packet.readShort();

		std::array<uint64_t, 4> weapons{};

		for (size_t i = 0; i < 4; i++)
		{
			weapons[i] = packet.readLong();
			packet.readLong();
		}

		conn->getPlayer()->getEquipmentManager()->setWeapons(characterIndex, weapons);
	}
};