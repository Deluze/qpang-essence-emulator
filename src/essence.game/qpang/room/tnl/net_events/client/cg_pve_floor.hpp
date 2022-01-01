#ifndef CG_PVE_FLOOR_H
#define CG_PVE_FLOOR_H

#include "GameNetEvent.h"
class CGPvEFloor final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U8 floorNumber;

	CGPvEFloor() : GameNetEvent{ CG_PVE_FLOOR, GuaranteedOrdered, DirClientToServer} {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&floorNumber);
	}

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			return;
		}

		roomSessionPlayer->setFloorNumber(floorNumber);
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEFloor>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEFloor);
};
#endif