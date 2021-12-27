#ifndef CG_MOVE_REPORT_H
#define CG_MOVE_REPORT_H

#include "GameNetEvent.h"

#include "Maps.h"
#include "gc_pve_move_npc.hpp"

class CGMoveReport : public GameNetEvent
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

	void pack(EventConnection* conn, BitStream* bstream) {}

	void unpack(EventConnection* conn, BitStream* bstream)
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

		if (roomSessionPlayer->isDead())
		{
			return;
		}

		roomSessionPlayer->setPosition({ xPos, yPos, zPos });

		// Convert pos to cell pos
		int cellX = ((xPos - -43.220) / 1.441);
		int cellZ = (((zPos - 40.823) / 1.399) * -1.f);
		//roomSessionPlayer->send<GCPvEMoveNpc>(50, (uint16_t)cellX, (uint16_t)cellZ);

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