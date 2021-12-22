#ifndef CG_PVE_RESPAWN_REQ_H
#define CG_PVE_RESPAWN_REQ_H

#include "GameNetEvent.h"
class CGPvERespawnReq final : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	bool flag; // 44

	CGPvERespawnReq() : GameNetEvent{ CG_PVE_RESPAWN_REQ, GuaranteedOrdered, DirClientToServer} {}

	void pack(EventConnection* conn, BitStream* bstream) override {}

	void handle(GameConnection* conn, Player::Ptr player) override
	{
		// TODO: Handle CGPvERespawnReq.
		std::cout << "Reveived an unhandled CGPvERespawnReq event." << std::endl;
	}

	void unpack(EventConnection* conn, BitStream* bstream) override
	{
		flag = bstream->readFlag();
	}

	void process(EventConnection* ps) override
	{
		post<CGPvERespawnReq>(ps);
	}

	TNL_DECLARE_CLASS(CGPvERespawnReq);
};
#endif