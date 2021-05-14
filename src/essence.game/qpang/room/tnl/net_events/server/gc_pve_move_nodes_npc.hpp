#ifndef GC_PVE_MOVE_NODES_NPC_HPP
#define GC_PVE_MOVE_NODES_NPC_HPP

#include "GameNetEvent.h"

class GCPvEMoveNodesNpc : public GameNetEvent
{
	typedef NetEvent Parent;
public:
	GCPvEMoveNodesNpc() : GameNetEvent{ GC_PVE_MOVE_NODES_NPC, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};

	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEMoveNodesNpc);
};

#endif // !GC_ARRANGED_ACCEPT_HPP