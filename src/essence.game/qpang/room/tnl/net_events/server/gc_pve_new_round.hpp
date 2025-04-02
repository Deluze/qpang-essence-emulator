#ifndef GC_PVE_NEW_ROUND_HPP
#define GC_PVE_NEW_ROUND_HPP

#include "GameNetEvent.h"

class GCPvENewRound : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvENewRound() : GameNetEvent{ GC_PVE_NEW_ROUND, Guaranteed, DirServerToClient } {}

	void pack(EventConnection* conn, BitStream* bstream) {}
	void unpack(EventConnection* conn, BitStream* bstream) {}
	void process(EventConnection* ps) {}

	TNL_DECLARE_CLASS(GCPvENewRound);
};

#endif // !GC_ARRANGED_ACCEPT_HPP