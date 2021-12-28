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

		const auto [x, y, z] = roomSessionPlayer->getPosition();

		// This means the player has gone underground (stage 1 sewers)
		if ((floorNumber == 0) && (y < 0.0f))
		{
			roomSessionPlayer->setFloorNumber(floorNumber);

			return;
		}

		// We compare with -1.0 here instead of 0.0 since a little correction is neccessary.
		if ((floorNumber == 1) && (y > -1.0f))
		{
			roomSessionPlayer->setFloorNumber(floorNumber);
		}
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEFloor>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEFloor);
};
#endif