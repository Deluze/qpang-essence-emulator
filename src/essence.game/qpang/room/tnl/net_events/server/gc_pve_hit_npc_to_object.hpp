#ifndef GC_PVE_HIT_NPC_TO_OBJECT_HPP
#define GC_PVE_HIT_NPC_TO_OBJECT_HPP

#include "GameNetEvent.h"

class GCPvEHitNpcToObject : public GameNetEvent
{
	typedef NetEvent Parent;
public:

	GCPvEHitNpcToObject() : GameNetEvent{ GC_PVE_HIT_NPC_TO_OBJECT, NetEvent::GuaranteeType::GuaranteedOrdered, NetEvent::DirAny } {};
	void pack(EventConnection* conn, BitStream* bstream) {};
	void unpack(EventConnection* conn, BitStream* bstream) {};
	void process(EventConnection* ps) {};

	TNL_DECLARE_CLASS(GCPvEHitNpcToObject);
};


#endif // !GC_ARRANGED_ACCEPT_HPP