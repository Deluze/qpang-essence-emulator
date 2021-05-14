#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"
#include "qpang/player/equipment/EquipmentManager.h"

#include "packets/lobby/outgoing/equipment/EquippedSkillCards.h"

class RequestEquippedSkillCards : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		std::array<InventoryCard, 3> skills = conn->getPlayer()->getEquipmentManager()->getSkillCards();

		conn->send(EquippedSkillCards(skills));
	}
};