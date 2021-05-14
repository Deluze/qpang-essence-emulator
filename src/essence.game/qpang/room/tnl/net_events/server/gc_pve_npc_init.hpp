#ifndef GC_PVE_NPC_INIT_HPP
#define GC_PVE_NPC_INIT_HPP

#include "GameNetEvent.h"

class GCPvENpcInit : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvENpcInit() : GameNetEvent{ GC_PVE_NPC_INIT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvENpcInit);
};


#endif // !GC_ARRANGED_ACCEPT_HPP