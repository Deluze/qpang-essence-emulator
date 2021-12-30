#ifndef CG_MOVE_REPORT_H
#define CG_MOVE_REPORT_H

#include "GameNetEvent.h"
#include "AABBHelper.h"

#include "Maps.h"

#include "gc_pve_move_npc.hpp"

class CGMoveReport final : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	U32 playerId; //88

	F32 xPos; //92
	F32 yPos; //96
	F32 zPos; //100

	// pitch & yaw?
	F32 unk_05; //104
	F32 unk_06; //108

	CGMoveReport() : GameNetEvent{ CG_MOVE_REPORT, GuaranteedOrdered, DirClientToServer } {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&playerId);
		bstream->read(&xPos);
		bstream->read(&yPos);
		bstream->read(&zPos);
		bstream->read(&unk_05);
		bstream->read(&unk_06);
	}

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			return;
		}

		if (roomPlayer->isSpectating())
		{
			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			return;
		}

		const auto roomSession = roomSessionPlayer->getRoomSession();

		if (roomSession == nullptr)
		{
			return;
		}

		if (roomSessionPlayer->isDead() || roomSessionPlayer->isPermanentlyDead())
		{
			return;
		}

		roomSessionPlayer->setPosition({ xPos, yPos, zPos });

		const auto room = roomPlayer->getRoom();
		if (room && room->getMode() == GameMode::PVE)
		{
			// Note: If this checking causes too much lag or whatever, move it back to PveAreaManager::tick method.
			for (const auto& area : roomSession->getPveAreaManager()->getAreas())
			{
				AABBHelper::isPositionInArea(roomSessionPlayer->getPosition(), area->getMinBound(), area->getMaxBound())
					&& area->getFloorNumber() == roomSessionPlayer->getFloorNumber()
					? area->onAreaEnter(roomSessionPlayer)
					: area->onAreaExit(roomSessionPlayer);
			}
		}

		// Convert pos to cell pos
		const int cellX = ((xPos - -43.220) / 1.441);
		const int cellZ = (((zPos - 40.823) / 1.399) * -1.f);
		roomSessionPlayer->send<GCPvEMoveNpc>(1337, (uint16_t)cellX, (uint16_t)cellZ);

		if (Maps::recordMoves)
		{
			if (Maps::debugWorldLayout[cellX][cellZ] != 0)
			{
				// set cell to 0 in worldlayout
				Maps::debugWorldLayout[cellX][cellZ] = 0;
				printf("Set { %d, %d } to moveable!\n", cellX, cellZ);
			}
		}
	}

	void process(EventConnection* ps) override
	{
		post<CGMoveReport>(ps);
	}

	TNL_DECLARE_CLASS(CGMoveReport);
};
#endif