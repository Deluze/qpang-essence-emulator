#ifndef CG_PVE_ROUND_START_H
#define CG_PVE_ROUND_START_H

#include "GameNetEvent.h"
#include "gc_pve_new_round.hpp"

class CGPvERoundStart : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	CGPvERoundStart() : GameNetEvent{ CG_PVE_ROUND_START, GuaranteedOrdered, DirClientToServer} {}

	void pack(EventConnection* conn, BitStream* bstream) override {}
	void unpack(EventConnection* conn, BitStream* bstream) override {}

	void handle(GameConnection* conn, const Player::Ptr player) override
	{
		// TODO: More checks need to happen here, but for now.. relay the new round event.
		player->getRoomPlayer()->getRoomSessionPlayer()->getRoomSession()->relayPlaying<GCPvENewRound>();
	}

	void process(EventConnection* ps) override
	{
		post<CGPvERoundStart>(ps);
	}

	TNL_DECLARE_CLASS(CGPvERoundStart);
};
#endif