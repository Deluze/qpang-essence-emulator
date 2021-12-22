#ifndef CG_PVE_FLOOR_H
#define CG_PVE_FLOOR_H

#include "GameNetEvent.h"
class CGPvEFloor final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U8 flag;

	CGPvEFloor() : GameNetEvent{ CG_PVE_FLOOR, GuaranteedOrdered, DirClientToServer} {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&flag);
	}

	void handle(GameConnection* conn, Player::Ptr player) override
	{
		// TODO: Handle CGPvEFloor.
		std::cout << "Reveived an unhandled CGPvEFloor event." << std::endl;
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEFloor>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEFloor);
};
#endif