#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"
#include "qpang/player/equipment/EquipmentManager.h"

#include "packets/lobby/outgoing/equipment/SendEquippedSkillCards.h"

class RequestEquippedSkillCards final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const std::array<InventoryCard, 3> skillsCards = conn->getPlayer()->getEquipmentManager()->getEquippedSkillCards();

		conn->send(SendEquippedSkillCards(skillsCards));
	}
};