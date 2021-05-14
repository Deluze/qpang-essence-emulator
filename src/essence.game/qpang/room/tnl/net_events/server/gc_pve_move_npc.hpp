#ifndef GC_PVE_MOVE_NPC_HPP
#define GC_PVE_MOVE_NPC_HPP

#include "GameNetEvent.h"

class GCPvEMoveNpc : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEMoveNpc() : GameNetEvent{ GC_PVE_MOVE_NPC, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEMoveNpc);
};

#endif // !GC_ARRANGED_ACCEPT_HPP