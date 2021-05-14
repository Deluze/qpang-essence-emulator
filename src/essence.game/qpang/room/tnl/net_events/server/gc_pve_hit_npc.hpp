#ifndef GC_PVE_HIT_NPC_HPP
#define GC_PVE_HIT_NPC_HPP

#include "GameNetEvent.h"

class GCPvEHitNpc : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	GCPvEHitNpc() : GameNetEvent{ GC_PVE_HIT_NPC, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEHitNpc);
};


#endif // !GC_ARRANGED_ACCEPT_HPP