#ifndef CG_PVE_AREA_TRIGGER_H
#define CG_PVE_AREA_TRIGGER_H

#include "GameNetEvent.h"
class CGPvEAreaTrigger final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	U32 flag;

	CGPvEAreaTrigger() : GameNetEvent{ CG_PVE_AREA_TRIGGER, GuaranteedOrdered, DirClientToServer } {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		bstream->read(&flag);
	}

	void handle(GameConnection* conn, Player::Ptr player) override
	{
		// TODO: Handle CGPvEAreaTrigger.
		std::cout << "Reveived an unhandled CGPvEAreaTrigger event." << std::endl;
	}

	void process(EventConnection* ps) override
	{
		post<CGPvEAreaTrigger>(ps);
	}

	TNL_DECLARE_CLASS(CGPvEAreaTrigger);
};
#endif