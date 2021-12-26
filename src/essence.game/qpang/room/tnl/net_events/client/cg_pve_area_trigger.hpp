#ifndef CG_PVE_AREA_TRIGGER_H
#define CG_PVE_AREA_TRIGGER_H

#include "GameNetEvent.h"

class CGPvEAreaTrigger final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 areaId;

	CGPvEAreaTrigger() : GameNetEvent{ CG_PVE_AREA_TRIGGER, GuaranteedOrdered, DirClientToServer } {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&areaId);
	}

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		std::cout << "Player " << player->getId() << " has triggered area with id " << areaId << std::endl;
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEAreaTrigger>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEAreaTrigger);
};
#endif