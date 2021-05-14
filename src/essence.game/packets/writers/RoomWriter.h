#pragma once

#include "core/communication/packet/ServerPacket.h"

#include "qpang/room/Room.h"

class RoomWriter
{
public:
	static void write(ServerPacket* packet, const Room::Ptr& room)
	{
		packet->writeInt(room->getHost());
		packet->writeShort(room->getPort());
		packet->writeInt(room->getId());
		packet->writeEmpty(2);
		packet->writeString(room->getName(), 30);
		packet->writeEmpty(14);
		packet->writeByte(room->getMap());
		packet->writeByte(room->getMode());
		packet->writeByte(room->getPassword().empty() ? 8 : 1);
		packet->writeByte(room->getState());
		packet->writeByte(room->getPlayerCount());
		packet->writeByte(room->getMaxPlayers());
		packet->writeEmpty(5);
		packet->writeFlag(room->isLevelLimited());
		packet->writeFlag(room->isTeamSorting());
		packet->writeFlag(room->isSkillsEnabled());
		packet->writeEmpty(2);
		packet->writeFlag(room->isMeleeOnly());
	}
};